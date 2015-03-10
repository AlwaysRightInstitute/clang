// (C) 2015 Helge Hess
// Derived from other runtime files.

#include "CGObjCSwifter.h"

using namespace clang;
using namespace CodeGen;

// yes, we just put it into a global namespace
namespace clang {
  namespace CodeGen {
    
    class CGObjCSwifter: public CGObjCSwifterVirtual {
      typedef CGObjCSwifterVirtual super;

     public:
      CGObjCSwifter(CodeGenModule &cgm);
      
      /* Methods & calls */
      
      // TODO
      
      /* Instance Variables */
      
      // TODO
      
      /* Class structure */
      
      // TODO
      
      /* Module */
      
      // TODO
    };
    
  } // CodeGen
} // clang
