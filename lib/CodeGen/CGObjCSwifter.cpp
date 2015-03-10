// (C) 2015 Helge Hess
// Derived from other runtime files.

#include "CGObjCSwifter.h"
#include "CodeGenModule.h"

using namespace clang;
using namespace CodeGen;

CGObjCRuntime *clang::CodeGen::CreateSwifterObjCRuntime(CodeGenModule &CGM) {
  switch (CGM.getLangOpts().ObjCRuntime.getKind()) {
    case ObjCRuntime::Swifter:
      return new CGObjCSwifter(CGM);
    
    case ObjCRuntime::GNUstep:
    case ObjCRuntime::GCC:
    case ObjCRuntime::ObjFW:
    case ObjCRuntime::FragileMacOSX:
    case ObjCRuntime::MacOSX:
    case ObjCRuntime::iOS:
      llvm_unreachable("these runtimes runtimes ain't no swift.");
  }
  llvm_unreachable("bad runtime");
}

CGObjCSwifter::CGObjCSwifter(CodeGenModule &cgm): super(cgm) {
  
}
