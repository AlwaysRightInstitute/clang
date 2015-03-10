WORKINGS
========

FLOW:
- Clang creates a SwifterHackAction
- SwifterHackAction passed SwifterHackConsumer to Clang
- Clang calls HandleTranslationUnit() on consumer after parsing
- Consumer creates a SwifterHackVisitor and uses that to walk the AST


- SwifterHack has the SwifterHackAction, which is the entry point of the plugin,
  called by Clang
  - this class is mapped to a cmdline name (in that .cpp file)
    - and then invoked via '-plugin <name>'

- one shared library can contain multiple plugins

- the action does two things:
  - a) parse additional plugin cmdline arguments
  - b) return an ASTConsumer, which is like a delegate for the parser

- the Swifter ASTConsumer is the parser delegate
  - can be triggered for top-level statements which got parsed
  - or after the whole AST is built
  - it can say whether the Sema needs to run first (TODO: what gives?)

- The Swifter ASTVisitor is just a helper object to traverse the AST
  and get called on specific node types
  

TEST HH
=======

export SWIFTER_CLANG="$HOME/hack-a-clang/build-clang-make"
export PATH="$SWIFTER_CLANG/bin:$PATH"

cd $HOME/hack-a-clang/

pushd $HOME/hack-a-clang/build-clang-make/examples/SwifterHack; make -s -j 8; popd; clang -Wno-objc-method-access -Wno-shadow-ivar -fsyntax-only -Xclang -fobjc-runtime=swifter -Xclang -load -Xclang ${SWIFTER_CLANG}/lib/SwifterHack.dylib -Xclang -plugin -Xclang swifter-hack -Xclang -ast-dump main.m

RUNNING
=======

Once the plugin is built, you can run it using:

export SWIFTER_CLANG="$HOME/hack-a-clang/clang-build/Debug"
export PATH="$SWIFTER_CLANG/bin:$PATH"

Note: when using -cc1 std-search pathes are not set.

clang -cc1 \
  -load ${SWIFTER_CLANG}/lib/SwifterHack.dylib \
  -plugin swifter-hack teststruct.c

clang -cc1 \
  -fobjc-runtime=swifter \
  -load ${SWIFTER_CLANG}/lib/SwifterHack.dylib \
  -plugin swifter-hack main.m

OPTIONS:
  -add-plugin <name>      Use the named plugin action in addition to the default action
  -load <dsopath>         Load the named plugin (dynamic shared object)
  -plugin-arg-<name> <arg>
                          Pass <arg> to plugin <name>
  -plugin <name>          Use the named plugin action instead of the default action (use "help" to list available options)


AST
===

clang -cc1 -ast-dump -fobjc-runtime=swifter -load ${SWIFTER_CLANG}/lib/SwifterHack.dylib main.m

clang -fsyntax-only -Xclang -ast-dump -Xclang -fobjc-runtime=swifter -Xclang -load -Xclang ${SWIFTER_CLANG}/lib/SwifterHack.dylib main.m


No AST when plugin is run

clang -cc1 -ast-dump -fobjc-runtime=swifter \
  -load ${SWIFTER_CLANG}/lib/SwifterHack.dylib -plugin swifter-hack main.m

clang -Wno-objc-method-access -Wno-shadow-ivar -fsyntax-only -Xclang \
  -fobjc-runtime=swifter -Xclang -load \
  -Xclang ${SWIFTER_CLANG}/lib/SwifterHack.dylib \
  -Xclang -plugin -Xclang swifter-hack -Xclang -ast-dump main.m


Xcode Build
===========

goes into ~/tmp/build/Debug
clang -Wno-objc-method-access -Wno-shadow-ivar -fsyntax-only \
  -Xclang -fobjc-runtime=swifter \
  -Xclang -load -Xclang $HOME/tmp/build/Debug/libSwifterHack.dylib \
  -Xclang -add-plugin -Xclang swifter-hack -Xclang -plugin-arg-swifter-hack=dump \
  -Xclang -Xclang -ast-dump main.m

Dox
===

https://code.google.com/p/chromium/wiki/WritingClangPlugins
https://kevinaboos.wordpress.com/2013/07/29/clang-tutorial-part-iii-plugin-example/
