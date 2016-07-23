#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>

 using namespace clang::tooling;
 using namespace llvm;

 static llvm::cl::OptionCategory MyToolCategory("mt options");

 static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
 static cl::extrahelp MoreHelp("\nMore Help info...");

int main(int argc, const char **argv)
{
     CommonOptionsParser OptionParser(argc, argv, MyToolCategory);
     ClangTool Tool(OptionParser.getCompilations(),
                    OptionParser.getSourcePathList());
    return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}
