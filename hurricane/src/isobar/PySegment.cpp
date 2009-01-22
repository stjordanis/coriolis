//
// $Id: PySegment.cpp,v 1.12 2008/02/07 17:09:42 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :      Jean-Paul.Chaput@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PySegment.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PySegment.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyVertical.h"
#include "hurricane/isobar/PyPoint.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Segment,segment,function)


// x=================================================================x
// |              "PySegment" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |               "PySegment" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PySegment_getSourceX,getSourceX,PySegment,Segment)
  DirectGetLongAttribute(PySegment_getSourceY,getSourceY,PySegment,Segment)
  DirectGetLongAttribute(PySegment_getWidth  ,getWidth,  PySegment,Segment)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PySegment_destroy, PySegment)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PySegment_getSourcePosition ()"

  static PyObject* PySegment_getSourcePosition ( PySegment *self )
  {
    trace << "PySegment_getSourcePosition()" << endl;

    METHOD_HEAD ( "Segment.SourcePosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( segment->getSourcePosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PySegment_getTargetPosition ()"

  static PyObject* PySegment_getTargetPosition ( PySegment *self )
  {
    trace << "PySegment_getTargetPosition()" << endl;

    METHOD_HEAD ( "Segment.TargetPosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( segment->getTargetPosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  // ---------------------------------------------------------------
  // PySegment Attribute Method table.

  PyMethodDef PySegment_Methods[] =
    { { "getSourceX"           , (PyCFunction)PySegment_getSourceX       , METH_NOARGS , "Return the Segment source X value." }
    , { "getSourceY"           , (PyCFunction)PySegment_getSourceY       , METH_NOARGS , "Return the Segment source Y value." }
    , { "getSourcePosition"    , (PyCFunction)PySegment_getSourcePosition, METH_NOARGS , "Return the Segment source point value." }
    , { "getTargetPosition"    , (PyCFunction)PySegment_getTargetPosition, METH_NOARGS , "Return the Segment target point value." }
    , { "getWidth"             , (PyCFunction)PySegment_getWidth         , METH_NOARGS , "Return the segment width." }
    , { "destroy"              , (PyCFunction)PySegment_destroy          , METH_NOARGS
                               , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                "PySegment" Object Methods                   |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Segment)
  PyTypeObjectLinkPyType(Segment)


# else  // End of Python Module Code Part.


// x=================================================================x
// |             "PySegment" Shared Library Code Part                |
// x=================================================================x




  // ---------------------------------------------------------------
  // PySegment Object Definitions.


  PyTypeObjectDefinitions(Segment)

# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
