// hh

#include "SwifterHackVisitor.h"
#include <iostream>

using namespace clang;

// Target output streams:
// - cout
// - llvm::errs()

SwifterHackVisitor::SwifterHackVisitor(SwifterHackConfig *_config) {
  this->config = _config;
}

bool SwifterHackVisitor::VisitObjCMethodDecl(ObjCMethodDecl *method) {
  Selector    sel  = method->getSelector();
  std::string name = sel.getAsString();
  
  if (method->hasBody())
    printf("Found method: %s\n", name.c_str());
  else
    printf("Found method prototype: %s\n", name.c_str());
  
  if (this->config != NULL && this->config->dumpMethodDecls)
    method->dump();
  
  return true;
}

bool SwifterHackVisitor::VisitObjCMessageExpr(ObjCMessageExpr *call) {
  
  ObjCMethodDecl *method = call->getMethodDecl();
  if (method == NULL)
    std::cout << "  Has no method!\n";

  if (this->config != NULL && this->config->dumpMethodCalls)
    call->dump();
  
  return true;
}
