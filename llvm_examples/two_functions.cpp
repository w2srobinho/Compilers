#include "llvm/IR/DerivedTypes.h"   //for types
#include "llvm/IR/IRBuilder.h"      //for Builder
#include "llvm/IR/LLVMContext.h"    //for Context
#include "llvm/IR/Module.h"         //for Module
#include "llvm/ExecutionEngine/ExecutionEngine.h" //for JIT
#include "llvm/ExecutionEngine/MCJIT.h"             //for JIT

#include "llvm/IR/Verifier.h" //for verification

#include <iostream>
#include <vector>

int main() {
    llvm::LLVMContext &Context = llvm::getGlobalContext();
    llvm::Module *OurModule;
    llvm::IRBuilder<> Builder(Context);

    OurModule = new llvm::Module("Our first intermediary code", Context);

    /***** Main: a function that gets no parameters and returns an integer *****/
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1FunctionType.html
     * static FunctionType * get (Type *Result, bool isVarArg)
     * FunctionType::get - Create a FunctionType taking no parameters. */
     llvm::Type* intType = llvm::Type::getInt1Ty(Context);
    llvm::FunctionType* typeOfMain = llvm::FunctionType::get(intType,false);

    /* http://llvm.org/docs/doxygen/html/classllvm_1_1Function.html
     * static Function * Create (FunctionType *Ty, LinkageTypes Linkage, const Twine &N="", Module *M=nullptr) */
    llvm::Function* ourMain = llvm::Function::Create(typeOfMain,llvm::Function::ExternalLinkage, "main", OurModule);

    /* http://llvm.org/docs/doxygen/html/classllvm_1_1BasicBlock.html
     * static BasicBlock * Create (LLVMContext &Context, const Twine &Name="", Function *Parent=nullptr, BasicBlock *InsertBefore=nullptr)
     * Creates a new BasicBlock.
     */
    llvm::BasicBlock *mainBB = llvm::BasicBlock::Create(Context, "entry", ourMain);

    /* http://llvm.org/docs/doxygen/html/classllvm_1_1IRBuilderBase.html
     * void SetInsertPoint (BasicBlock *TheBB)
     * This specifies that created instructions should be appended to the end of the specified block.
     */
    Builder.SetInsertPoint(mainBB);

    /*** Constants are all unified ***/
    /* http://llvm.org/docs/doxygen/html/classllvm_1_1ConstantInt.html
     * static ConstantInt *	get (LLVMContext &Context, const APInt &V)
     * Return a ConstantInt with the specified value and an implied Type.
     * APInt = Arbitrary Precision Integer */
    llvm::ConstantInt* Ten = llvm::ConstantInt::get(Context, llvm::APInt(64,10));
    auto One = llvm::ConstantInt::get(Context, llvm::APInt(64, 1));
    auto Zero = llvm::ConstantInt::get(Context, llvm::APInt(64, 0));

    /* static ConstantInt *	get (IntegerType *Ty, uint64_t V, bool isSigned=false)
     * Return a ConstantInt with the specified integer value for the specified type. */
    llvm::Constant* NegativeOne = llvm::ConstantInt::get( intType, -1, true);

    /*** Lets work with another function named Booleana!! ***/
    /* Average takes three integers as input and returns one */
    llvm::Type* boolType = llvm::Type::getInt1Ty(Context);
    std::vector<llvm::Type*> BooleanParameters(3, boolType);
    llvm::FunctionType* typeOfBoolean = llvm::FunctionType::get(boolType, BooleanParameters, false);

    /* We name it 'average' */
    llvm::Function* ourBoolean = llvm::Function::Create(typeOfBoolean,llvm::Function::ExternalLinkage, "booleana", OurModule);

    /* Now we name its parameters 'a' and 'b' */
    llvm::Function::arg_iterator params = ourBoolean->arg_begin();
    params->setName("x");
    ++params;
    params->setName("y");
    ++params;
    params->setName("z");

    /* Our function will have its own basic block to add code */
    llvm::BasicBlock *averageBB = llvm::BasicBlock::Create(Context, "bool", ourBoolean);
    Builder.SetInsertPoint(averageBB);

    /* Now for the code: return (x&y)|(x&z)|(NOT z & NOT y) */
    auto x = ourBoolean->arg_begin();
    auto y = ++(ourBoolean->arg_begin());
    auto z = ++(++(ourBoolean->arg_begin()));
    auto tmp1 = Builder.CreateAnd(x, y, "andtmp");
    auto tmp2 = Builder.CreateAnd(x, z, "andtmp");
    auto notz = Builder.CreateNot(z, "notz");
    auto noty = Builder.CreateNot(y, "noty");
    auto tmp3 = Builder.CreateAnd(notz, noty, "andtmp");
    tmp1 = Builder.CreateOr(tmp1, tmp2, "ortmp");
    tmp1 = Builder.CreateOr(tmp1, tmp3, "ortmp");
    Builder.CreateRet(tmp1);

    llvm::verifyFunction(*ourBoolean); //Checks if everything is okay with our function

    /*** Now we return to our main to call the average function ***/
    Builder.SetInsertPoint(mainBB);

    /* Lets call booleana with false and false and true */
    std::vector<llvm::Value*> arguments;
    arguments.push_back(llvm::ConstantInt::getFalse(Context));
    arguments.push_back(llvm::ConstantInt::getFalse(Context));
    arguments.push_back(llvm::ConstantInt::getTrue(Context));
    auto boolReturn = Builder.CreateCall(ourBoolean, arguments, "callbool");

    /* And we return the average at the end*/
    Builder.CreateRet(boolReturn);

    llvm::verifyFunction(*ourMain); //Checks if everything is okay with our function

    /*** Lets print the intermediary representation generated ***/
    OurModule->dump();
    //ourMain->dump();

    /*** Now lets compute it with a just in time (JIT) compiler ***/
    llvm::ExecutionEngine* OurExecutionEngine;
    std::string Error;
    LLVMInitializeNativeTarget(); //target = generates code for my processor
    OurExecutionEngine = llvm::EngineBuilder(OurModule).setErrorStr(&Error).create();
    if (!OurExecutionEngine) {
        fprintf(stderr, "Could not create OurExecutionEngine: %s\n", Error.c_str());
        exit(1);
    }

    // JIT our main. It returns a function pointer.
    void *mainPointer = OurExecutionEngine->getPointerToFunction(ourMain);
    // Translate the pointer and run our main to get its results
    int (*result)() = (int (*)())(intptr_t)mainPointer;
    std::cout << "Result of our main = " << result() << std::endl;
}
