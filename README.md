Clang-Swifter
=============

Hack-add a simple thirdparty Objective-C runtime to Clang. Documents just the
process, not the actual implementation.

Let's see whether I have the persistence to complete this 'tutorial' ;-)
Feedback is welcome.

### Intro

Assume you came up with your own, personal, Objective-C runtime. All you miss is
a decent compiler ...
Now Clang claims to be a very modular and easy to hack C compiler (specifically
compared to GCC). Let's see how it goes ... ;->

Recommendation: If you want to understand Swift, it is a really good idea to
read through the LLVM tutorials. It gives you an insight on where they are
coming from. Pretty much the same reason why it's a good idea to have a basic
understanding of assembler when you are working in C, or of the JVM when you do
Java.


### Setup Xcode dev environment

We want to code the Clang stuff in Xcode, mostly for the debugger. This
is easy to setup because the LLVM/Clang built system is based on 
[Cmake](http://www.cmake.org).
Cmake is a makefile generator, which can generate Makefiles, VisualStudio
projects, and Xcode ones. Not in a particulary good way, but OK.

OSX doesn't come with cmake included, so I'd recommend to install it via
[Homebrew](http://brew.sh):

    brew install cmake

Next checkout LLVM and Clang, in _separate_ directories. Not in one as suggested
by the Clang [online documentation](http://clang.llvm.org/get_started.html).
Both repositories are BIG (LLVM 326MB, Clang 177MB!).
I also suggest checking out a specific release branch (I chose release_36), as
Clang trunk is a moving target (even the release branches receive updates).

    cd ~
    mkdir hack-a-clang
    cd hack-a-clang
    
    git clone -b release_36 http://llvm.org/git/llvm.git
    git clone -b release_36 http://llvm.org/git/clang.git
    
    cd llvm/projects
    git clone -b release_36 http://llvm.org/git/compiler-rt.git
    cd ../..

Next we create an LLVM build directory, let cmake create Makefiles and build
Clang.

    cd ~/hack-a-clang
    mkdir llvm-build
    cd llvm-build
    
    cmake -G "Unix Makefiles" -DLLVM_ENABLE_CXX1Y:BOOL=OFF ../llvm
      # throws warning, which we choose to ignore
    
    time make -s -j 8
      # grab ☕️, then another ☕️ and one more ☕️

And go ahead to Clang. You can build Clang using makefiles like above, but
we want to hack in Xcode. *Important*: Don't forget to setup the path to point
to the LLVM bin directory.
Looks like this:

    cd ~/hack-a-clang
    export PATH="`pwd`/llvm-build/bin:${PATH}"
    mkdir clang-build
    cd clang-build
    
    cmake -G "Xcode" ../clang
      # throws warning, which we choose to ignore
    
    time xcodebuild -target clang
      # grab ☕️, then another ☕️ and one more ☕️

In case this worked right, you can now test Clang. This includes a set of
useful calls to the clang binary:

    cd ~/hack-a-clang
    export PATH="`pwd`/clang-build/Debug/bin:${PATH}"
    echo 'main() { printf("Hello World!\n"); return 0; }' > hello.m
    
    which clang # check it's picking up your clang
      # e.g. /Users/helge/hack-a-clang/clang-build/Debug/bin/clang
    
    clang --help
    clang hello.m -fsyntax-only
    clang hello.m -S -emit-llvm -o - # print LLVM assembly code for hello.c
    clang hello.m -S -emit-llvm -O3 -o -
    clang hello.m -S -O3 -o -        # Intel assembly code (compiled by LLVM)
    clang -Xclang -ast-dump -fsyntax-only hello.m

If all is good, you can fire up clang in Xcode:

    cd ~/hack-a-clang
    cd clang-build
    open Clang.xcodeproj
      # while Xcode indexes the source, grab ☕️, then another ☕️

The project is HUGE. First, switch the target from 'ALL_BUILD' to 'clang'
(somewhere at the bottom of the list).

**Important**: Everytime cmake detects something it considers a change, it'll
regenerate your Xcode project. So while you can setup new groups and such in
Xcode, this is a lossy process. Don't invest too much time into that.

Next, lets setup Xcode for debugging. Go to the target popup and select
'Edit Scheme'. In the 'Run' section add those arguments:

    -cc1
    $(HOME)/hack-a-clang/hello.m
    -S
    -emit-llvm
    -o -

Press Cmd-r to run it, and it should output the LLVM code for hello.m.

Great, we have a working development environment.


### Intro 2

So what is Clang. Clang is a set of things. It's a preprocessor system,
it is a C/C++/ObjC/ObjC++ parser and code generator. And it is a 'compiler
driver', aka the shell tool which wraps the compilation stages. The latter
makes the whole thing a bit weird, because both, the compiler and the driver
are named 'clang'.

So the steps performed by the driver (when you call 'clang hello.m -o hello')
are essentially:
- take a file to compile
- scan & preprocess it (convert source text into tokens)
- parse it into an AST (a tree representation of the source),
  this is actually two separate modules in clang:
  - the parser (detect token sequences, feed the Sema)
  - the Sema (semantic analyzer), which constructs the AST
- CodeGen: emit LLVM IR code from the AST
- let LLVM compile the IR
- link the native code

To generate Objective-C code, the code generation needs to be adjusted. It
takes the AST and emits the LLVM IR (intermediate representation) code for this.
In the case of Objective-C this is stuff like message sends `[obj doIt]`,
runtime meta data, or class/selector lookups (e.g. `@selector(doIt:)`).


Remember that I said that 'clang' is both the driver executable and the
compiler? That turns out to be very confusing when you use the web to search
for options the tools take ... If you run 'clang' from the 'bin' build
directory, you run the driver.
If you want to pass options to the compiler, there are two ways to do it.
Either prepend every option with -Xclang:

    clang -Xclang -ast-dump -fsyntax-only hello.m

or invoke the compiler directly via -cc1, eg:

    clang -cc1 -ast-dump hello.m


### Reading Materials

Upfront: When working on stuff like this I always take notes in
[VoodooPad](https://plausible.coop/voodoopad/).
I paste in links, I paste in sample source code, I take notes in it. All
automagically cross linked.

OK, below a set of links which you might want to read.

#### LLVM

You should definitely read through the
[Kaleidoscope tutorial](http://llvm.org/docs/tutorial/).
It gives you a solid understanding of how to write a simple compiler which
targets LLVM.

JIT tutorial: [A first function](http://llvm.org/releases/2.6/docs/tutorial/JITTutorial1.html) and
[A more complicated function](http://llvm.org/releases/2.6/docs/tutorial/JITTutorial2.html).
Pretty good.

Another great one is
[Mapping High-Level Constructs to LLVM IR](http://llvm.lyngvig.org/Articles/Mapping-High-Level-Constructs-to-LLVM-IR),
which shows how C/C++ constructs look in LLVM assembly (IR) code.
Note that this one is different from Kaleidoscope in that it doesn't show
how you create the IR at runtime. It shows the textual representation.

LLVM IR is build around something called SSA - short for
[Static single assignment form](http://en.wikipedia.org/wiki/Static_single_assignment_form),
you'll see references to this all over the place.
Don't be too afraid when reading the article, you don't have to understand too
much of it. Just remember that there is something called a 'Phi' function.

Advanced, look at this later:
- getelementptr instruction (GEP)
  - http://llvm.org/docs/GetElementPtr.html
  - http://llvm.org/docs/LangRef.html#getelementptr-instruction

#### Clang

Clang hacking docu: [Hacking on Clang](http://clang.llvm.org/hacking.html)

[Kevin Boos Tutorial](https://kevinaboos.wordpress.com/2013/07/23/clang-tutorial-part-i-introduction/),
in three parts. Very nice.
Note: this talks about Clang plugins and tooling. Which is absolutely worth
reading (do it!), but neither technology is suitable for implementing a new
runtime backend. For that you actually have to hack clang.

[Introduction to the Clang AST](http://clang.llvm.org/docs/IntroductionToTheClangAST.html),
OK to scan it, I didn't watch the video.

Others:
- [What makes Clang so special](http://www.codergears.com/Blog/?p=679)
- [Creating and using Clang plugin with Xcode](http://railsware.com/blog/2014/02/28/creation-and-using-clang-plugin-with-xcode/)
- [Create a working compiler with the LLVM framework](http://www.ibm.com/developerworks/library/os-createcompilerllvm1/), [Part2](http://www.ibm.com/developerworks/library/os-createcompilerllvm2/)


### Hack in a new runtime

Let's start hacking in support for a new runtime. As mentioned the primary
thing which needs to be adjusted is the code generator which transforms
the AST into LLVM IR.

Open the Xcode project and browse to:

    Sources / Clang libraries / clangCodeGen / Source Files

This is the code generator.

#### Step 1: Create a new generator class for the runtime

Now Clang already comes with support for a set of different runtimes. Those
are grouped into two families: Mac and GNU, both have a set of variants
(eg fragile vs non-fragile etc.)

The runtime specific code generation parts are separated out in subclasses of
the 'CGObjCRuntime' class, e.g. there is CGObjCGCC (the runtime coming with the
GCC compiler), CGObjCGNUstep (the modern GNUstep runtime), CGObjCObjFW (the
runtime of the ObjFW kit).

So, it is kinda obvious what we need to do first: Create a new runtime CodeGen
class.

Ours will be of neither family, it'll sideline both. Check the GIT commit to
see what needs to be done. To avoid having to implement every single code
generator from the beginning, there is a base class which just contains the 
'virtuals' of CGObjCRuntime, with an abort() ([CGObjCSwifterVirtual.h](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CGObjCSwifterVirtual.h)).
Our class itself lives in [CGObjCSwifter.h](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CGObjCSwifter.h)/[CGObjCSwifter.cpp](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CGObjCSwifter.cpp).
Right now it's rather empty.

To add the new class to the compilation process, add CGObjCSwifter.cpp to
[CMakeLists.txt](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CMakeLists.txt).
*Again*: Any change here is going to recreate your xcodeproj, all your own
file groups and such will be gone.


#### Step 2: Hook up the new runtime class to the driver

OK, now we have our abort'y implementation of a new ObjC runtime backend. When
compiling a file, we need to tell Clang that we want to use it.
To select the runtime, Clang uses the `-fobjc-runtime=xyz` option, e.g.
`-fobjc-runtime=gnustep` or `-fobjc-runtime=ios`.
What we want is `-fobjc-runtime=swifter`.

In Clang this spreads out over various files with plenty of redundancy. But the
core is the ObjCRuntime.Kind enum in 
[ObjCRuntime.h](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/include/clang/Basic/ObjCRuntime.h)
class of the Basic library (so much for the famous Clang 'modularity' ...).
*Important*: This class just represents the capabilities of a specific runtime
version, do *not* mix it up with the `CGObjCRuntime` class, which is the
base class for the ObjC code generator.
This object is accessed in various places within Clang, usually in a codepath like:
    
    if (LangOpts.ObjC1) {
      if (LangOpts.ObjCRuntime.isNonFragile()) {
        Builder.defineMacro("__OBJC2__");

See the git commit for the details, but those are files we patch just to add
Swifter to the enum:
[ObjCRuntime.h](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/include/clang/Basic/ObjCRuntime.h),
[ObjCRuntime.cpp](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/Basic/ObjCRuntime.cpp),
[CGException.cpp](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CGException.cpp),
[CGObjCGNU.cpp](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CGObjCGNU.cpp),
[CGObjCMac.cpp](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CGObjCMac.cpp),
[CodeGenModule.cpp](https://github.com/AlwaysRightInstitute/clang/blob/r36-swifter/lib/CodeGen/CodeGenModule.cpp).

That's it. Add `-fobjc-runtime=swifter` to the Clang arguments in the Xcode
scheme.
This should make Cmd-r properly crash in the abort() call in
CGObjCSwifterVirtual::ModuleInitFunction.

Great. All the infrastructure is in place now, we 'just' need to generate
the code :-)


### To be continued ...

Continue text here ;-)




### C++

Given that you are working on something Objective-C, there is a good chance you
might miss some C++ basics. And Clang is using pretty much any feature C++
provides.

##### private namespaces

If you see a namespace declaration w/o a name like this:

    namespace {
      ...
    }

it creates a 'private' namespace. Think of it like 'static' in C. The contents
will only be visible in the current translation unit.

##### auto

Let the compiler automatically derive the type, Swift-like. This is
crap to read as you can't see the type :-) Sample:

    ExprAST::dump(out << "var", ind);
    for (const auto &NamedVar : VarNames) {
      NamedVar.second->dump(indent(out, ind) 
        << NamedVar.first << ':', ind + 1);
    }

##### ampersand reference parameters

You'll see a lot of objects being passed around by reference. Like:

    static uint64_t LookupFieldBitOffset(CodeGen::CodeGenModule &CGM, ...) {
      CGM.doThis()
    }

Instead of

    static uint64_t LookupFieldBitOffset(CodeGen::CodeGenModule *CGM, ...) {
      CGM->doThat()
    }

It's essentially the same.

##### const'ness

There is a lot of it. You know

    const char * const ptr;

Dangerous [const_cast](http://en.cppreference.com/w/cpp/language/const_cast)
hack-a-round:

    void SwifterHackMethodDeclIfNecessary
      (CodeGen::CodeGenModule &cgm, const ObjCMethodDecl *cmethod)
    {
      ObjCMethodDecl *method = const_cast<ObjCMethodDecl *>(cmethod);
    }

##### std::string

This is the C++ string class (actually a template instance of
basic_string<char>). Clang uses additional string objects, StringRef,
SmallString and Twine.
Note that C strings automatically cast to std::strings etc.
When declaring a method argument, use
[StringRef](http://llvm.org/docs/ProgrammersManual.html#the-stringref-class).

Important method: stdString.c_str(). Gives you the char* string.

Difference to NSString? Plenty. Not usually passed as a reference, but as a
value object.

##### std::pair

Just a pair of two objects. Sample:

    class VarExprAST : public ExprAST {
      std::vector<std::pair<std::string, ExprAST *> > VarNames;

This is an array of string/ptr pairs.

##### std::vector

Again, unlike ObjC, usually allocated as a value object. Sample:

    std::vector<std::string> titles;
    titles.push_back("Hello");
    titles.push_back("World!");
    for (int i = 0, cnt = titles.size(); i < cnt; i++)
      printf("title: %s\n", titles[i].c_str());

You just have to love C++ ...

##### RTTI

It is worth noting that Clang doesn't use C++ RTTI. Instead it has it's own
dynamic type system. With own 'isa', 'dyn_cast' and 'cast' implementations.

Sample:

    if (isa<ObjCMethodDecl>)(this))
      return false;
    
    // Swift: if let ai as? AllocationInst { } ...
    if (AllocationInst *ai = dyn_cast<AllocationInst>(val))
      ..

##### Iterators

Quite honestly I didn't really get them yet. Must be similar to the Swift
range stuff I assume :-)


### Funny Sidenotes

Compilation of LLVM and Clang takes a looong time. Working on it feels a lot
like Unix development in the year 2000. Instead of having instant compiles on
save, you actually can go and grab a coffee while stuff rebuilds.

#### Compile Timings

Machines (all OSX 10.10):
- zAir   (MacBook Air non-PCIe, i7 2GHz, 8GB RAM)
- Zini   (Mac Mini Quad Core i7, 16 GB, Software RAID-0 on OWC SSDs)
- ZPro   (can Mac Pro 4-Core D300 32GB 1TB PCIe)
- ZeePro (can Mac Pro 4-Core D300 16GB 512GB PCIe)

##### LLVM (w/o embedded clang)

| Command                  | Machine | Real   | User   | Sys   |
|--------------------------|---------|--------|--------|-------|
| make -j 16               | Zini    | 6m16s  | 40m27s | 3m4s  |
| make -j 16               | ZPro    | 5m8s   | 34m24  | 2m17s |
| make -j  8               | ZPro    | 5m3s   | 31m51s | 2m8s  |
| make -j  8               | zAir    | 11m5s  | 38m2s  | 2m19s |
| make -j  8               | ZeePro  | 4m40s  | 32m3s  | 2m    |

##### Clang

| Command                  | Machine | Real   | User   | Sys   |
|--------------------------|---------|--------|--------|-------|
| xcodebuild -target clang | ZPro    | 4m32s  | 25m18s | 1m57s |
| xcodebuild -target clang | zAir    | 9m51s  | 30m7s  | 2m10s |
| make -j 4                | Zini    | 4m52s  | 17m28s | 1m19s |
| xcodebuild -target clang | ZeePro  | 4m26s  | 25m28s | 1m48s |
| make -j 7                | ZeePro  | 4m32s  | 29m49s | 2m34s |


###Why?!

Someone bothered me to do this, and I was actually interested in how all this
works. So I gave it a try. I'm not actually using any of this.

###Contact

[@helje5](http://twitter.com/helje5) | helge@alwaysrightinstitute.com

![](http://www.alwaysrightinstitute.com/ARI.png)
