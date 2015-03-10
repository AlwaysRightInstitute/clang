// (C) 2015 Helge Hess
// Derived from other runtime files.

#include "CGObjCRuntime.h"

using namespace clang;
using namespace CodeGen;

// yes, we just put it into a global namespace
namespace clang {
  namespace CodeGen {
    
    class CGObjCSwifterVirtual: public CGObjCRuntime {
      typedef CGObjCRuntime super;
     public:
      CGObjCSwifterVirtual(CodeGenModule &cgm): CGObjCRuntime(cgm) {
      }
    
     public: /* pure virtual funcs */
      #define SWIFTER_NOT_IMPLEMENTED_CRASH_NULL abort(); return NULL;
      #define SWIFTER_NOT_IMPLEMENTED_CRASH_VOID abort(); return;
    
      /// Generate the function required to register all Objective-C components
      /// in this compilation unit with the runtime library.
      virtual llvm::Function *ModuleInitFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      /// Get a selector for the specified name and type values. The
      /// return value should have the LLVM type for pointer-to
      /// ASTContext::getObjCSelType().
      virtual llvm::Value *GetSelector
        (CodeGenFunction &CGF, Selector Sel, bool lval=false)
        override
      {
        // HH: in NeXT, this generates a static structure (EmitSelector)
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      /// Get a typed selector.
      virtual llvm::Value *GetSelector
        (CodeGenFunction &CGF, const ObjCMethodDecl *Method)
        override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      /// Generate a category.  A category contains a list of methods (and
      /// accompanying metadata) and a list of protocols.
      virtual void GenerateCategory(const ObjCCategoryImplDecl *OCD) override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
    
      /// Generate a class structure for this class.
      virtual void GenerateClass(const ObjCImplementationDecl *OID) override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
    
      /// Register an class alias.
      virtual void RegisterAlias(const ObjCCompatibleAliasDecl *OAD) override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
    
      /// Generate an Objective-C message send operation.
      ///
      /// \param Method - The method being called, this may be 0L if synthesizing
      /// a property setter or getter.
      virtual CodeGen::RValue GenerateMessageSend
        (CodeGen::CodeGenFunction &CGF,
         ReturnValueSlot ReturnSlot, QualType ResultType,
         Selector Sel, llvm::Value *Receiver,
         const CallArgList       &CallArgs,
         const ObjCInterfaceDecl *Class  = nullptr,
         const ObjCMethodDecl    *Method = nullptr) override
      {
        // TODO: HH
        abort();
        return RValue();
      }
    
      /// Generate an Objective-C message send operation to the super
      /// class initiated in a method for Class and with the given Self
      /// object.
      ///
      /// \param Method - The method being called, this may be 0L if synthesizing
      /// a property setter or getter.
      virtual CodeGen::RValue GenerateMessageSendSuper
        (CodeGen::CodeGenFunction &CGF,
         ReturnValueSlot ReturnSlot, QualType ResultType,
         Selector                Sel,
         const ObjCInterfaceDecl *Class,
         bool                    isCategoryImpl,
         llvm::Value             *Self,
         bool                    IsClassMessage,
         const CallArgList       &CallArgs,
         const ObjCMethodDecl    *Method = nullptr) override
      {
        // TODO: HH
        abort();
        return RValue();
      }

      /// Generate a function preamble for a method with the specified
      /// types.
    
      // FIXME: Current this just generates the Func definition, but really this
      // should also be generating the loads of the parameters, as the runtime
      // should have full control over how parameters are passed.
      virtual llvm::Function *GenerateMethod
        (const ObjCMethodDecl *OMD, const ObjCContainerDecl *CD) override
      {
        // hh: called by CodeGenFunction::StartObjCMethod()
        //     this only registers the Function prototype representing the
        //     method. It does NOT generate the actual code
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      /// GetClass - Return a reference to the class for the given
      /// interface decl.
      virtual llvm::Value *GetClass
        (CodeGenFunction &CGF, const ObjCInterfaceDecl *OID) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }

      virtual llvm::Constant *BuildByrefLayout
        (CodeGen::CodeGenModule &CGM, QualType T) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      virtual llvm::GlobalVariable *GetClassGlobal
        (const std::string &Name, bool Weak = false) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }


      /* memory access */
    
      virtual void EmitObjCIvarAssign
        (CodeGen::CodeGenFunction &CGF,
         llvm::Value *src, llvm::Value *dest, llvm::Value *ivarOffset)
        override
      {
        /// EmitObjCIvarAssign - Code gen for assigning to a __strong object.
        /// objc_assign_ivar (id src, id *dst, ptrdiff_t ivaroffset)
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
    
      virtual LValue EmitObjCValueForIvar
        (CodeGen::CodeGenFunction &CGF, QualType ObjectTy,
         llvm::Value *BaseValue,
         const ObjCIvarDecl *Ivar, unsigned CVRQualifiers) override
      {
        /// EmitObjCValueForIvar - Code Gen for ivar reference.
        abort(); // TODO: HH
        return LValue();
      }
      virtual llvm::Value *EmitIvarOffset
        (CodeGen::CodeGenFunction &CGF,
         const ObjCInterfaceDecl *Interface, const ObjCIvarDecl *Ivar) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }

      virtual void EmitObjCGlobalAssign
        (CodeGen::CodeGenFunction &CGF, llvm::Value *src, llvm::Value *dest,
         bool threadlocal=false)
        override
      {
        /// EmitObjCGlobalAssign - Code gen for assigning to a __strong object.
        /// objc_assign_global (id src, id *dst)
        // HH: is this only for ARC or in non-ARC, are all values considered
        //     strong?
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
      virtual void EmitObjCStrongCastAssign
        (CodeGen::CodeGenFunction &CGF, llvm::Value *src, llvm::Value *dest)
        override
      {
        /// EmitObjCStrongCastAssign - Code gen for assigning to a __strong cast
        /// object.
        /// objc_assign_strongCast (id src, id *dst)
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }

    
      /* constant strings */

      /// Generate a constant string object.
      virtual llvm::Constant *GenerateConstantString(const StringLiteral *)
        override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    

      /* protocols */
    
      /// Generate the named protocol.  Protocols contain method metadata but no
      /// implementations.
      virtual void GenerateProtocol(const ObjCProtocolDecl *OPD) override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }

      /// Emit the code to return the named protocol as an object, as in a
      /// \@protocol expression.
      virtual llvm::Value *GenerateProtocolRef
        (CodeGenFunction &CGF, const ObjCProtocolDecl *OPD) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }

    
      /* foreach */
    
      /// EnumerationMutationFunction - Return the function that's called by the
      /// compiler when a mutation is detected during foreach iteration.
      virtual llvm::Constant *EnumerationMutationFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }


      /* properties */
    
      /// Return the runtime function for getting properties.
      virtual llvm::Constant *GetPropertyGetFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      /// Return the runtime function for setting properties.
      virtual llvm::Constant *GetPropertySetFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      /// Return the runtime function for optimized setting properties.
      virtual llvm::Constant *GetOptimizedPropertySetFunction
        (bool atomic, bool copy) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      // API for atomic copying of qualified aggregates in getter.
      virtual llvm::Constant *GetGetStructFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
      // API for atomic copying of qualified aggregates in setter.
      virtual llvm::Constant *GetSetStructFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
      /// API for atomic copying of qualified aggregates with non-trivial copy
      /// assignment (c++) in setter.
      virtual llvm::Constant *GetCppAtomicObjectSetFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
      /// API for atomic copying of qualified aggregates with non-trivial copy
      /// assignment (c++) in getter.
      virtual llvm::Constant *GetCppAtomicObjectGetFunction() override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }

    
      /* exception stuff */
    
      /// Get the type constant to catch for the given ObjC pointer type.
      /// This is used externally to implement catching ObjC types in C++.
      /// Runtimes which don't support this should add the appropriate
      /// error to Sema.
      virtual llvm::Constant *GetEHType(QualType T) override {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    
      virtual void EmitSynchronizedStmt
        (CodeGen::CodeGenFunction &CGF, const ObjCAtSynchronizedStmt &S)
        override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
      virtual void EmitTryStmt
        (CodeGen::CodeGenFunction &CGF, const ObjCAtTryStmt &S) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
      virtual void EmitThrowStmt
        (CodeGen::CodeGenFunction &CGF, const ObjCAtThrowStmt &S,
         bool ClearInsertionPoint=true) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }


      /* Garbage Collector / ARC */

      virtual llvm::Value *EmitObjCWeakRead
        (CodeGen::CodeGenFunction &CGF, llvm::Value *AddrWeakObj) override
      {
        /// EmitObjCWeakRead - Code gen for loading value of a __weak
        /// object: objc_read_weak (id *src)
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
      virtual void EmitObjCWeakAssign
        (CodeGen::CodeGenFunction &CGF, llvm::Value *src, llvm::Value *dest)
        override
      {
        /// EmitObjCWeakAssign - Code gen for assigning to a __weak object.
        /// objc_assign_weak (id src, id *dst)
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }

      virtual void EmitGCMemmoveCollectable
        (CodeGen::CodeGenFunction &CGF,
         llvm::Value *DestPtr, llvm::Value *SrcPtr, llvm::Value *Size) override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_VOID; // TODO: HH
      }
    
      virtual llvm::Constant *BuildGCBlockLayout
        (CodeGen::CodeGenModule &CGM, const CodeGen::CGBlockInfo &blockInfo)
        override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
      virtual llvm::Constant *BuildRCBlockLayout
        (CodeGen::CodeGenModule &CGM, const CodeGen::CGBlockInfo &blockInfo)
        override
      {
        SWIFTER_NOT_IMPLEMENTED_CRASH_NULL; // TODO: HH
      }
    };
    
  } // CodeGen
} // clang
