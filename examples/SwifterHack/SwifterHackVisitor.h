// hh

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "SwifterHackConfig.h"

using namespace clang;

// HH: This is just an object which can traverse the AST tree, and calls
//     callbacks for specific AST node types.
class SwifterHackVisitor : public RecursiveASTVisitor<SwifterHackVisitor> {
 protected:
  SwifterHackConfig *config;
  
 public:
  SwifterHackVisitor(SwifterHackConfig *config);
  
 public:
  bool VisitObjCMethodDecl (ObjCMethodDecl  *method);
  bool VisitObjCMessageExpr(ObjCMessageExpr *call);
};
