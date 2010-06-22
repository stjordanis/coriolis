
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <string>

#include  "vlsisapd/configuration/Configuration.h"
#include  "hurricane/Cell.h"
#include  "crlcore/Utilities.h"
#include  "kite/Configuration.h"
#include  "kite/KiteEngine.h"



namespace Kite {


  using std::cout;
  using std::cerr;
  using std::endl;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::Error;
  using Hurricane::Technology;


// -------------------------------------------------------------------
// Class  :  "Kite::Configuration".


  Configuration::Configuration ( Katabatic::Configuration* base )
    : Katabatic::Configuration()
    , _base               (base)
    , _postEventCb        ()
    , _edgeCapacityPercent(Cfg::getParamPercentage("kite.edgeCapacity", 80.0)->asDouble())
    , _expandStep         (Cfg::getParamPercentage("kite.expandStep"  ,100.0)->asDouble())
    , _ripupLimits        ()
    , _ripupCost          (Cfg::getParamInt("kite.ripupCost"   ,      3)->asInt())
    , _eventsLimit        (Cfg::getParamInt("kite.eventsLimit" ,4000000)->asInt())
  {
    _ripupLimits[BorderRipupLimit]     = Cfg::getParamInt("kite.borderRipupLimit"     ,26)->asInt();
    _ripupLimits[StrapRipupLimit]      = Cfg::getParamInt("kite.strapRipupLimit"      ,16)->asInt();
    _ripupLimits[LocalRipupLimit]      = Cfg::getParamInt("kite.localRipupLimit"      , 7)->asInt();
    _ripupLimits[GlobalRipupLimit]     = Cfg::getParamInt("kite.globalRipupLimit"     , 5)->asInt();
    _ripupLimits[LongGlobalRipupLimit] = Cfg::getParamInt("kite.longGlobalRipupLimit" , 5)->asInt();
  }


  Configuration::Configuration ( const Configuration& other, Katabatic::Configuration* base )
    : Katabatic::Configuration()
    , _base               (base)
    , _postEventCb        (other._postEventCb)
    , _edgeCapacityPercent(other._edgeCapacityPercent)
    , _expandStep         (other._expandStep)
    , _ripupLimits        ()
    , _ripupCost          (other._ripupCost)
    , _eventsLimit        (other._eventsLimit)
  {
    if ( _base == NULL ) _base = other._base->clone();

    _ripupLimits[BorderRipupLimit]     = other._ripupLimits[BorderRipupLimit];
    _ripupLimits[StrapRipupLimit]      = other._ripupLimits[StrapRipupLimit];
    _ripupLimits[LocalRipupLimit]      = other._ripupLimits[LocalRipupLimit];
    _ripupLimits[GlobalRipupLimit]     = other._ripupLimits[GlobalRipupLimit];
    _ripupLimits[LongGlobalRipupLimit] = other._ripupLimits[LongGlobalRipupLimit];
  }


  Configuration::~Configuration ()
  { }


  Configuration* Configuration::clone () const
  { return new Configuration(*this); }


  Configuration* Configuration::clone ( KiteEngine* kite ) const
  { return new Configuration(*this,kite->base()->getKatabaticConfiguration()); }


  bool  Configuration::isGMetal ( const Layer* layer ) const
  { return _base->isGMetal(layer); }


  size_t  Configuration::getDepth () const
  { return _base->getDepth(); }


  size_t  Configuration::getAllowedDepth () const
  { return _base->getAllowedDepth(); }


  size_t  Configuration::getLayerDepth ( const Layer* layer ) const
  { return _base->getLayerDepth(layer); }


  RoutingGauge* Configuration::getRoutingGauge () const
  { return _base->getRoutingGauge(); }


  RoutingLayerGauge* Configuration::getLayerGauge ( size_t depth ) const
  { return _base->getLayerGauge(depth); }


  const Layer* Configuration::getRoutingLayer ( size_t depth ) const
  { return _base->getRoutingLayer(depth); }


  Layer* Configuration::getContactLayer ( size_t depth ) const
  { return _base->getContactLayer(depth); }


  DbU::Unit  Configuration::getExtensionCap () const
  { return _base->getExtensionCap(); }


  float  Configuration::getSaturateRatio () const
  { return _base->getSaturateRatio(); }


  DbU::Unit  Configuration::getGlobalThreshold () const
  { return _base->getGlobalThreshold(); }


  void  Configuration::setAllowedDepth ( size_t allowedDepth )
  { _base->setAllowedDepth(allowedDepth); }


  void  Configuration::setSaturateRatio ( float ratio )
  { _base->setSaturateRatio(ratio); }


  void  Configuration::setGlobalThreshold ( DbU::Unit threshold )
  { _base->setGlobalThreshold(threshold); }


  void  Configuration::setRipupLimit ( unsigned int type, unsigned int limit )
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("setRipupLimit(): Bad ripup limit index: %ud (> %ud)."
                   ,type,RipupLimitsTableSize) << endl;
      return;
    }

    _ripupLimits [ type ] = limit;
  }


  void  Configuration::setEdgeCapacityPercent ( float percent )
  {
    if ( percent > 1.0 )
      throw Error("Configuration::setEdgeCapacityPercent(): edge capacity ratio greater than 1.0 (%.1f)."
                 ,percent);

    _edgeCapacityPercent = percent;
  }


  unsigned int  Configuration::getRipupLimit ( unsigned int type ) const
  {
    if ( type >= RipupLimitsTableSize ) {
      cerr << Error("getRipupLimit(): Bad ripup limit index: %u (> %u)."
                   ,type,RipupLimitsTableSize) << endl;
      return 0;
    }
    return _ripupLimits[type];
  }


  void  Configuration::print ( Cell* cell ) const
  {
    cout << "  o  Configuration of ToolEngine<Kite> for Cell <" << cell->getName() << ">" << endl;
    cout << Dots::asPercentage("     - Global router edge capacity"        ,_edgeCapacityPercent) << endl;
    cout << Dots::asPercentage("     - GCell aggregation threshold (delta)",_expandStep) << endl;
    cout << Dots::asULong     ("     - Events limit (iterations)"          ,_eventsLimit) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, borders"               ,_ripupLimits[BorderRipupLimit]) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, straps"                ,_ripupLimits[StrapRipupLimit]) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, locals"                ,_ripupLimits[LocalRipupLimit]) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, globals"               ,_ripupLimits[GlobalRipupLimit]) << endl;
    cout << Dots::asUInt      ("     - Ripup limit, long globals"          ,_ripupLimits[LongGlobalRipupLimit]) << endl;

    _base->print ( cell );
  }


  string  Configuration::_getTypeName () const
  {
    return "Configuration";
  }


  string  Configuration::_getString () const
  {
    ostringstream  os;

    os << "<" << _getTypeName() << " " << getRoutingGauge()->getName() << ">";

    return os.str();
  }


  Record* Configuration::_getRecord () const
  {
    Record* record = _base->_getRecord();
  //record->add ( getSlot ( "_rg"       ,  _rg      ) );
                                     
    return record;
  }



}  // End of Kite namespace.
