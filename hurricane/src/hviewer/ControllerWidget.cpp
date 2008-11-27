
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ControllerWidget.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QFrame>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QCheckBox>
#include  <QComboBox>
#include  <hurricane/DataBase.h>
#include  <hurricane/Cell.h>
#include  <hurricane/viewer/Graphics.h>
#include  <hurricane/viewer/CellWidget.h>
#include  <hurricane/viewer/GraphicsWidget.h>
#include  <hurricane/viewer/PaletteWidget.h>
#include  <hurricane/viewer/DisplayFilterWidget.h>
#include  <hurricane/viewer/NetlistWidget.h>
#include  <hurricane/viewer/SelectionWidget.h>
#include  <hurricane/viewer/InspectorWidget.h>
#include  <hurricane/viewer/ControllerWidget.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::ControllerTab".


  ControllerTab::ControllerTab ( QWidget* parent )
    : QWidget(parent)
    , _cellWidget(NULL)
  { }


  void  ControllerTab::setCellWidget ( CellWidget* cellWidget )
  {
    _cellWidget = cellWidget;
  }


  void  ControllerTab::setCell ( Cell* )
  { }


  void  ControllerTab::updateTab ()
  { }


  void  ControllerTab::cellPreModificate ()
  { }


  void  ControllerTab::cellPostModificate ()
  { }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabGraphics".


  TabGraphics::TabGraphics ( QWidget* parent )
    : ControllerTab(parent)
    , _graphics    (new GraphicsWidget())
  {
    _graphics->setObjectName ( "controller.tabGraphics.graphics" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 0, 0, 0, 0 );
    wLayout->addWidget ( _graphics );
    setLayout ( wLayout );
  }


  void  TabGraphics::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      if ( getCellWidget() ) {
        connect ( _graphics, SIGNAL(styleChanged()), getCellWidget(), SLOT(redraw()) );
      }
    }
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabDisplayFilter".


  TabDisplayFilter::TabDisplayFilter ( QWidget* parent )
    : ControllerTab (parent)
    , _displayFilter(new DisplayFilterWidget())
  {
    _displayFilter->setObjectName ( "controller.tabDisplayFilter.graphics" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 0, 0, 0, 0 );
    wLayout->addWidget ( _displayFilter );
    setLayout ( wLayout );
  }


  void  TabDisplayFilter::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      _displayFilter->setCellWidget ( cellWidget );
    }
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabPalette".


  TabPalette::TabPalette ( QWidget* parent )
    : ControllerTab(parent)
    , _palette     (new PaletteWidget())
  {
    _palette->setObjectName ( "controller.tabPalette.palette" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 0, 0, 0, 0 );
    wLayout->addWidget ( _palette );
    setLayout ( wLayout );
  }


  void  TabPalette::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() )
      getCellWidget()->detachFromPalette ();

    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      if ( getCellWidget() ) {
        getCellWidget()->bindToPalette ( _palette );
      }
    }
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabNetlist".


  TabNetlist::TabNetlist ( QWidget* parent )
    : ControllerTab  (parent)
    , _netlistBrowser(new NetlistWidget())
    , _synchronize   (new QCheckBox())
  {
    _netlistBrowser->setObjectName ( "controller.tabNetlist.netlistBrowser" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 0, 10, 0 );
    wLayout->setSpacing ( 0 );

    _synchronize->setText    ( tr("Synchronize Netlist") );
    _synchronize->setChecked ( false );
    _synchronize->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
    connect ( _synchronize, SIGNAL(toggled(bool)), this, SLOT(setSynchronize(bool)) );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins ( 0, 0, 0, 0 );
    commands->addStretch ();
    commands->addWidget  ( _synchronize );
    commands->addStretch ();
    wLayout->addLayout   ( commands );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    wLayout->addWidget ( separator );
    wLayout->addWidget ( _netlistBrowser );

    setLayout ( wLayout );
  }


  void  TabNetlist::setSynchronize ( bool state )
  {
    if ( state && getCellWidget() ) {
      _netlistBrowser->setCell<SimpleNetInformations> ( getCellWidget()->getCell() );
    } else {
      _netlistBrowser->setCell<SimpleNetInformations> ( NULL );
    }
  }


  void  TabNetlist::setCell ( Cell* cell )
  {
    setSynchronize ( _synchronize->isChecked() );
  }


  void  TabNetlist::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      if ( getCellWidget() ) {
        connect ( getCellWidget(), SIGNAL(cellChanged(Cell*))     , this           , SLOT(setCell(Cell*)) );
        connect ( _netlistBrowser, SIGNAL(netSelected(const Net*)), getCellWidget(), SLOT(select(const Net*)) );
      }
      setSynchronize ( _synchronize->isChecked() );
    }
  }


  void  TabNetlist::cellPreModificate ()
  {
    _netlistBrowser->setCell<SimpleNetInformations> ( NULL );
  }


  void  TabNetlist::cellPostModificate ()
  {
    setSynchronize ( _synchronize->isChecked() );
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabSelection".


  TabSelection::TabSelection ( QWidget* parent )
    : ControllerTab(parent)
    , _selection   (new SelectionWidget())
  {
    _selection->setObjectName ( "controller.tabSelection.selection" );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 10, 10, 0 );
    wLayout->addWidget ( _selection );
    setLayout ( wLayout );
  }


  void  TabSelection::setCell ( Cell* cell )
  {
    _selection->setSelection ( set<Selector*>() );
  }


  void  TabSelection::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget ( cellWidget );
      if ( getCellWidget() ) {
        connect ( getCellWidget() , SIGNAL(cellChanged(Cell*))
                , this            , SLOT(setCell(Cell*)) );
        connect (  getCellWidget(), SIGNAL(selectionChanged(const set<Selector*>&,Cell*))
                ,  _selection     , SLOT  (setSelection    (const set<Selector*>&,Cell*)) );
        connect (  _selection     , SIGNAL(occurrenceToggled(Occurrence,bool))
                ,  getCellWidget(), SLOT  (toggleSelect     (Occurrence,bool)) );
        connect (  getCellWidget(), SIGNAL(occurrenceToggled(Occurrence))
                ,  _selection     , SLOT  (toggleSelection  (Occurrence)) );
        connect (  _selection     , SIGNAL(cumulativeToggled     (bool))
                ,  getCellWidget(), SLOT  (setCumulativeSelection(bool)) );
        connect (  _selection     , SIGNAL(showSelectionToggled(bool))
                ,  getCellWidget(), SLOT  (setShowSelection    (bool)) );
        connect (  getCellWidget(), SIGNAL(showSelectionToggled(bool))
                ,  _selection     , SLOT  (setShowSelection    (bool)) );
        connect (  _selection     , SIGNAL(selectionCleared())
                ,  getCellWidget(), SLOT  (unselectAll     ()) );
      }
    }
  }


  void  TabSelection::updateTab ()
  {
//     if ( _cellWidget && _cellWidget->getCell() )
//       _selection->setSelection ( _cellWidget->getSelectorSet(), _cellWidget->getCell() );
//     else
//       _selection->setSelection ( set<Selector*>() );
  }


  void  TabSelection::cellPreModificate ()
  {
    _selection->setSelection ( set<Selector*>() );
  }


  void  TabSelection::cellPostModificate ()
  {
  //updateTab ();
    if ( getCellWidget() && getCellWidget()->getCell() )
      _selection->setSelection ( getCellWidget()->getSelectorSet(), getCellWidget()->getCell() );
    else
      _selection->setSelection ( set<Selector*>() );
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::TabInspector".


  TabInspector::TabInspector ( QWidget* parent )
    : ControllerTab       (parent)
    , _inspectorWidget    (new InspectorWidget())
    , _bookmarks          (new QComboBox())
    , _selectionOccurrence()
    , _updateFromSelection(true)
  {
    _inspectorWidget->setObjectName ( "controller.tabInspector.inspectorWidget" );

    connect ( _bookmarks, SIGNAL(currentIndexChanged(int)), this, SLOT(bookmarkChanged(int)) );

    QVBoxLayout* wLayout  = new QVBoxLayout ();
    wLayout->setContentsMargins ( 10, 10, 10, 0 );
  //wLayout->setSpacing ( 0 );

    _bookmarks->addItem ( tr("<Disabled>" ) );
    _bookmarks->addItem ( tr("<DataBase>" ) );
    _bookmarks->addItem ( tr("<Cell None>") );
    _bookmarks->addItem ( tr("<Selection>") );

    QHBoxLayout* commands = new QHBoxLayout ();
    commands->setContentsMargins ( 0, 0, 0, 0 );
    commands->addStretch ();
    commands->addWidget  ( _bookmarks );
    commands->addStretch ();
    wLayout->addLayout   ( commands );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    wLayout->addWidget ( separator );
    wLayout->addWidget ( _inspectorWidget );

    setLayout ( wLayout );
  }


  void  TabInspector::setCell ( Cell* cell )
  {
    _bookmarks->setItemText ( 2, getString(cell).c_str() );
    if ( _bookmarks->currentIndex() == 2 )
      emit bookmarkChanged ( 2 );
  }


  void  TabInspector::setCellWidget ( CellWidget* cellWidget )
  {
    if ( getCellWidget() != cellWidget ) {
      ControllerTab::setCellWidget( cellWidget );
      if ( getCellWidget() ) {
        connect ( getCellWidget(), SIGNAL(cellChanged(Cell*)), this, SLOT(setCell(Cell*)) );
      }
    }
  }


  void  TabInspector::bookmarkChanged ( int index )
  {
    switch ( index ) {
      case 0: _inspectorWidget->setRootRecord ( NULL ); break;
      case 1: _inspectorWidget->setRootRecord ( getRecord(DataBase::getDB()) ); break;
      case 2:
        if ( getCellWidget() && getCellWidget()->getCell() )
          _inspectorWidget->setRootRecord ( getRecord(getCellWidget()->getCell()) );
        break;
      case 3:
        if ( getCellWidget() && getCellWidget()->getCell() )
          _inspectorWidget->setRootRecord ( getRecord(_selectionOccurrence) );
        break;
    }
  }


  void  TabInspector::updateTab ()
  {
    if ( _updateFromSelection && (_bookmarks->currentIndex() == 3) ) {
      _inspectorWidget->setRootRecord ( getRecord(_selectionOccurrence) );
    }
    _updateFromSelection = false;
  }


  void  TabInspector::setSelectionOccurrence ( Occurrence& occurrence )
  {
    _updateFromSelection = true;
    _selectionOccurrence = occurrence;
  }


  void  TabInspector::cellPreModificate ()
  {
    _selectionOccurrence = Occurrence();
    if ( _bookmarks->currentIndex() > 1 )
      _inspectorWidget->setRootRecord ( NULL );
  }


  void  TabInspector::cellPostModificate ()
  {
    if ( ( _bookmarks->currentIndex() == 2 ) && getCellWidget() && getCellWidget()->getCell() )
      _inspectorWidget->setRootRecord ( getRecord(getCellWidget()->getCell()) );
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::ControllerWidget".


  ControllerWidget::ControllerWidget ( QWidget* parent )
    : QTabWidget       (parent)
    , _cellWidget      (NULL)
    , _tabGraphics     (new TabGraphics())
    , _tabPalette      (new TabPalette())
    , _tabDisplayFilter(new TabDisplayFilter())
    , _tabNetlist      (new TabNetlist())
    , _tabSelection    (new TabSelection())
    , _tabInspector    (new TabInspector())
  {
    setObjectName  ( "controller" );
    setAttribute   ( Qt::WA_QuitOnClose, false );
    setWindowTitle ( tr("Controller") );

  //connect ( _netlistBrowser, SIGNAL(destroyed()), this, SLOT(netlistBrowserDestroyed()) );

    _tabGraphics     ->setObjectName ( "controller.graphics"      );
    _tabPalette      ->setObjectName ( "controller.palette"       );
    _tabDisplayFilter->setObjectName ( "controller.displayFilter" );
    _tabNetlist      ->setObjectName ( "controller.tabNetlist"    );
    _tabSelection    ->setObjectName ( "controller.tabSelection"  );
    _tabInspector    ->setObjectName ( "controller.tabInspector"  );

    addTab ( _tabGraphics      , "Look"        );
    addTab ( _tabDisplayFilter , "Filter"      );
    addTab ( _tabPalette       , "Layers&&Gos" );
    addTab ( _tabNetlist       , "Netlist"     );
    addTab ( _tabSelection     , "Selection"   );
    addTab ( _tabInspector     , "Inspector"   );

    connect ( this, SIGNAL(currentChanged(int)), this, SLOT(updateTab(int)) );
  //connect ( _tabNetlist->getNetlistBrowser(), SIGNAL(netSelected(const Net*))
  //        , _tabSelection                   , SLOT(setUpdateFromNetlist(const Net*)) );
    connect ( _tabSelection->getSelection()   , SIGNAL(inspect(Occurrence&))
            , _tabInspector                   , SLOT(setSelectionOccurrence(Occurrence&)) );
                                        
    resize ( 540, 540 );
  }


  void  ControllerWidget::setCellWidget ( CellWidget* cellWidget )
  {
    _cellWidget = cellWidget;
    if ( _cellWidget ) {
      for ( int i=0 ; i<count() ; ++i )
        (static_cast<ControllerTab*>(widget(i)))->setCellWidget ( _cellWidget );

      connect ( _cellWidget, SIGNAL(cellChanged(Cell*))   , this, SLOT(cellChanged(Cell*))   );
      connect ( _cellWidget, SIGNAL(cellPreModificated()) , this, SLOT(cellPreModificate())  );
      connect ( _cellWidget, SIGNAL(cellPostModificated()), this, SLOT(cellPostModificate()) );
    }
  }


  void  ControllerWidget::cellChanged ( Cell* cell )
  { }


  void  ControllerWidget::updateTab ( int index )
  {
    (static_cast<ControllerTab*>(widget(index)))->updateTab ();
  }


  void  ControllerWidget::cellPreModificate ()
  {
    for ( int i=0 ; i<count() ; ++i )
      (static_cast<ControllerTab*>(widget(i)))->cellPreModificate ();
  }


  void  ControllerWidget::cellPostModificate ()
  {
    for ( int i=0 ; i<count() ; ++i )
      (static_cast<ControllerTab*>(widget(i)))->cellPostModificate ();
  }


}  // End of Hurricane namespace.
