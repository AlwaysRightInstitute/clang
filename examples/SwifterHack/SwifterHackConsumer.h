// hh

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"

#include "SwifterHackConfig.h"

using namespace clang;

// HH: this is like a parsing delegate, it get some top-level parsing state,
//     or you can wait until the whole translation-unit got parsed, and
//     then traverse the AST
class SwifterHackConsumer : public ASTConsumer {
    
 protected:
  SwifterHackConfig config;
  
 public:
  SwifterHackConsumer(SwifterHackConfig config);
    
 public:
  virtual bool HandleTopLevelDecl   (DeclGroupRef DG);
  virtual void HandleTranslationUnit(ASTContext &Ctx);
    
};
