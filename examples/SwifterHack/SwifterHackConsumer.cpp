// hh

#include "SwifterHackConsumer.h"
#include <iostream>

#include "clang/AST/ASTContext.h"

using namespace clang;

#include "SwifterHackVisitor.h"

// Target output streams:
// - cout
// - llvm::errs()

SwifterHackConsumer::SwifterHackConsumer(SwifterHackConfig _config): ASTConsumer() {
  this->config = _config;

#if 0  
  if (this->config.dumpMethodCalls)
    printf("calls are on.\n");
  if (this->config.dumpMethodDecls)
    printf("decls are on.\n");
  if (this->config.dumpTreeAfter)
    printf("tree after is on.\n");
  if (this->config.dumpTreeBefore)
    printf("tree before is on.\n");
#endif
}

bool SwifterHackConsumer::HandleTopLevelDecl(DeclGroupRef DG) {
#if 0 // just the demo, we don't want that.
  for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; ++i) {
    const Decl *D = *i;
    if (const NamedDecl *ND = dyn_cast<NamedDecl>(D))
      std::cout << "Swifter top-level-decl: \"" 
        << ND->getNameAsString() << "\"\n";
  }
#endif
  return true;
}

void SwifterHackConsumer::HandleTranslationUnit(ASTContext &ctx) {
  SwifterHackVisitor visitor(&this->config); // C++ is so weird ...
  
  std::cout << "\n\n\nDone with parsing.\n";
  TranslationUnitDecl *tu = ctx.getTranslationUnitDecl();

  if (this->config.dumpTreeBefore)
    tu->dump();
  
  visitor.TraverseDecl(tu);
  
  if (this->config.dumpTreeAfter)
    tu->dump();
}
