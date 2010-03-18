
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./AllianceFramework.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_ALLIANCE_FRAMEWORK__
#define  __CRL_ALLIANCE_FRAMEWORK__


#include  <map>

#include  "crlcore/Environment.h"
#include  "crlcore/AllianceLibrary.h"
#include  "crlcore/Catalog.h"
#include  "crlcore/ParsersDrivers.h"


namespace CRL {


  class RoutingGauge;
  class CellGauge;


  class AllianceFramework {

    public:
    // Constructors.
      static AllianceFramework* create                   ();
    // Destructors.
             void               destroy                  ();
    // Accessors.
      static AllianceFramework* get                      ();
             string             getPrint                 () const;
    // Predicates.
      inline bool               isPOWER                  ( const char*   name );
      inline bool               isPOWER                  ( const string& name );
      inline bool               isPOWER                  ( const Name&   name );
      inline bool               isGROUND                 ( const char*   name );
      inline bool               isGROUND                 ( const string& name );
      inline bool               isGROUND                 ( const Name&   name );
      inline bool               isCLOCK                  ( const char*   name );
      inline bool               isCLOCK                  ( const string& name );
      inline bool               isCLOCK                  ( const Name&   name );
      inline bool               isOBSTACLE               ( const char*   name );
      inline bool               isOBSTACLE               ( const string& name );
      inline bool               isOBSTACLE               ( const Name&   name );
    // Accessors.
      inline Environment*       getEnvironment           ();
      inline Catalog*           getCatalog               ();
      inline const Name&        getParentLibraryName     () const;
      inline Library*           getParentLibrary         ();
             Library*           getLibrary               ( unsigned index );
             AllianceLibrary*   getAllianceLibrary       ( const Name& path, bool& flag );
             RoutingGauge*      getRoutingGauge          ( const Name& name="" );
             CellGauge*         getCellGauge             ( const Name& name="" );
      inline const Name         getDefaultCGPinLayerName () const;
    // Modifiers.
             void               addRoutingGauge          ( RoutingGauge* );
             void               addCellGauge             ( CellGauge* );
    // Cell Management.
             Cell*              getCell                  ( const string& name
                                                         , unsigned int  mode
                                                         , unsigned int  depth=(unsigned int)-1 );
             Cell*              createCell               ( const string& name );
             void               saveCell                 ( Cell* cell , unsigned int mode );
             unsigned int       loadLibraryCells         ( Library* library );
             unsigned int       loadLibraryCells         ( const Name& name );

    // Internals - Attributes.
    protected:
      static const Name          _parentLibraryName;
      static AllianceFramework*  _singleton;
             Environment         _environment;
             ParsersMap          _parsers;
             DriversMap          _drivers;
             Catalog             _catalog;
             AllianceLibraries   _libraries;
             Library*            _parentLibrary;
             RoutingGauge*       _defaultRoutingGauge;
             map<const Name,RoutingGauge*>
                                 _routingGauges;
             map<const Name,CellGauge*>
                                 _cellGauges;
             CellGauge*          _defaultCellGauge;

    // Internals - Constructors.
                                 AllianceFramework       ();
                                 AllianceFramework       ( const AllianceFramework& );
              AllianceFramework& operator=               ( const AllianceFramework& );
    // Internals - Destructors.
                                ~AllianceFramework       ();
    // Internals - Methods.
              bool               _readLocate             ( const string& file, unsigned int mode, bool isLib=false );
              bool               _writeLocate            ( const string& file, unsigned int mode, bool isLib=false );

  };

  inline bool         AllianceFramework::isPOWER          ( const char*   name ) { return _environment.isPOWER(name); }
  inline bool         AllianceFramework::isPOWER          ( const string& name ) { return isPOWER(name.c_str()); }
  inline bool         AllianceFramework::isPOWER          ( const Name&   name ) { return isPOWER(getString(name)); }
  inline bool         AllianceFramework::isGROUND         ( const char*   name ) { return _environment.isGROUND(name); }
  inline bool         AllianceFramework::isGROUND         ( const string& name ) { return isGROUND(name.c_str()); }
  inline bool         AllianceFramework::isGROUND         ( const Name&   name ) { return isGROUND(getString(name)); }
  inline bool         AllianceFramework::isCLOCK          ( const char*   name ) { return _environment.isCLOCK(name); }
  inline bool         AllianceFramework::isCLOCK          ( const string& name ) { return isCLOCK(name.c_str()); }
  inline bool         AllianceFramework::isCLOCK          ( const Name&   name ) { return isCLOCK(getString(name)); }
  inline bool         AllianceFramework::isOBSTACLE       ( const char*   name ) { return _environment.isOBSTACLE(name); }
  inline bool         AllianceFramework::isOBSTACLE       ( const string& name ) { return isOBSTACLE(name.c_str()); }
  inline bool         AllianceFramework::isOBSTACLE       ( const Name&   name ) { return isOBSTACLE(getString(name)); }
  inline Environment* AllianceFramework::getEnvironment   () { return &_environment; }
  inline Catalog*     AllianceFramework::getCatalog       () { return &_catalog; }
  inline const Name&  AllianceFramework::getParentLibraryName
                                                          () const { return _parentLibraryName; }
  inline Library*     AllianceFramework::getParentLibrary () { return  _parentLibrary; }
// TEMPORARY.
  inline const Name   AllianceFramework::getDefaultCGPinLayerName
                                                          () const { return "CALU1"; }


} // End of CRL namespace.


#endif // __CRL_ALLIANCE_FRAMEWORK__