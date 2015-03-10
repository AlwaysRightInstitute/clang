//===- SwifterHack.cpp ------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

#include "SwifterHackConsumer.h"
#include "SwifterHackConfig.h"

/* the plugin entry point - aka the 'Action' */

class SwifterHackAction : public PluginASTAction {
  
 protected:
  SwifterHackConfig config;
  
 public:
  SwifterHackAction(): PluginASTAction() {
    memset(&this->config, 0, sizeof(this->config));
  }
  
 protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef)
  {
    // HH: give Clang our AST handler
    // Note: with --add-plugin I think the Consumer might live longer
    //       than the Action, hence the need to copy the config
    return llvm::make_unique<SwifterHackConsumer>(config);
  }
  
  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string>& args)
  {
    // HH: how are args passed in?:
    // "Additional parameters for the plugins can be passed with -plugin-arg"
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
      llvm::errs() << "SwifterHack arg = " << args[i] << "\n";

      // Example error handling.
      if (args[i] == "-an-error") {
        DiagnosticsEngine &D = CI.getDiagnostics();
        unsigned DiagID = D.getCustomDiagID(DiagnosticsEngine::Error,
                                            "invalid argument '%0'");
        D.Report(DiagID) << args[i];
        return false;
      }
    }
    if (args.size() && args[0] == "help")
      PrintHelp(llvm::errs());

    return true;
  }
  void PrintHelp(llvm::raw_ostream& ros) {
    ros << "Help for SwifterHack plugin goes here\n";
  }

};

static FrontendPluginRegistry::Add<SwifterHackAction>
X("swifter-hack", "hack AST for Swifter runtime");
