#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/ASTContext.h>

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

static const std::string loopName { "forLoop" };
static const std::string incrementName { "increment" };
static const std::string condVarName { "condVar" };
static const std::string initVarName { "initVar" };
StatementMatcher LoopMatcher = forStmt(
        hasLoopInit(declStmt(
                hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0))))
                .bind(initVarName)))),
        hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(
                        to(varDecl(hasType(isInteger())).bind(incrementName)))))),
        hasCondition(binaryOperator(
                hasOperatorName("<"),
                hasLHS(ignoringParenImpCasts(declRefExpr(
                            to(varDecl(hasType(isInteger())).bind(condVarName))))),
                hasRHS(expr(hasType(isInteger())))))
    ).bind(loopName);

static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
    return First && Second &&
        First->getCanonicalDecl() == Second->getCanonicalDecl();
}

class LoopPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(MatchFinder::MatchResult const& Result) {
      ASTContext * context = Result.Context;
      const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>(loopName);
      if (!FS || !context->getSourceManager().isInMainFile(FS->getForLoc()))
          return;

      const VarDecl * incVar = Result.Nodes.getNodeAs<VarDecl>(incrementName);
      const VarDecl * condVar = Result.Nodes.getNodeAs<VarDecl>(condVarName);
      const VarDecl * initVar = Result.Nodes.getNodeAs<VarDecl>(initVarName);

      if (!areSameVariable(incVar, condVar) || !areSameVariable(incVar, initVar))
          return;

      FS->getForLoc().print(llvm::outs(), context->getSourceManager());
      llvm::outs() << "\n";
  }
};


 static llvm::cl::OptionCategory MyToolCategory("mt options");

 static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
 static cl::extrahelp MoreHelp("\nMore Help info...");

int main(int argc, const char **argv)
{
     CommonOptionsParser OptionParser(argc, argv, MyToolCategory);
     ClangTool Tool(OptionParser.getCompilations(),
                    OptionParser.getSourcePathList());
     LoopPrinter printer;
     MatchFinder finder;
     finder.addMatcher(LoopMatcher, &printer);
     return Tool.run(newFrontendActionFactory(&finder).get());
}
