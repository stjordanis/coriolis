// ****************************************************************************************************
// File: Collection.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment LIP6 & BULL versions

#ifndef HURRICANE_COLLECTION
#define HURRICANE_COLLECTION

#include "Locator.h"
#include "Filter.h"

namespace Hurricane {

template<class Type> class GenericCollection;
template<class Type, class SubType> class SubTypeCollection;
template<class Type> class SubSetCollection;



// ****************************************************************************************************
// Collection declaration
// ****************************************************************************************************

template<class Type> class Collection : public NestedSlotAdapter {
// *************************************************************

// Constructors
// ************

    protected: Collection()
    // ********************
    {
    }

    private: Collection(const Collection& collection);
    // ***********************************************
    // not implemented to forbid copy construction
    // ***********************************************

// Destructor
// **********

    public: virtual ~Collection()
    // **************************
    {
    }

// Operators
// *********

    private: Collection& operator=(const Collection& collection);
    // **********************************************************
    // not implemented to forbid assignment
    // **********************************************************

// Accessors
// *********

    public: virtual Collection<Type>* getClone() const = 0;

    public: virtual Locator<Type>* getLocator() const = 0;

    public: virtual unsigned getSize() const
    // *************************************
    {
        unsigned size = 0;
        // we use a GenericLocator to delete the locator allocated by getLocator()
        GenericLocator<Type> locator = getLocator();
        while (locator.isValid()) {
            size++;
            locator.progress();
        }
        return size;
    }

    public: Type getFirst() const
    // **************************
    {
        // we use a GenericLocator to delete the locator allocated by getLocator()
        return GenericLocator<Type>(getLocator()).getElement();
    }

    public: GenericCollection<Type> getSubSet(const Filter<Type>& filter) const
    // ************************************************************************
    {
        return SubSetCollection<Type>(*this, filter);
    }

    public: template<class SubType> GenericCollection<SubType> getSubSet() const
    // *************************************************************************
    {
        return SubTypeCollection<Type, SubType>(this);
    }

    public: template<class SubType>
              GenericCollection<SubType> getSubSet(const Filter<SubType>& filter) const
    // ******************************************************************************
    {
        return getSubSet<SubType>().getSubSet(filter);
    }

// Predicates
// **********

    public: bool IsEmpty() const
    // *************************
    {
        // we use a GenericLocator to delete the locator allocated by getLocator()
        return !GenericLocator<Type>(getLocator()).isValid();
    }

// Utilitarians
// ************

    public: void fill(list<Type>& list) const
    // **************************************
    {
        GenericLocator<Type> locator = getLocator();
        while (locator.isValid()) {
            list.push_back(locator.getElement());
            locator.progress();
        }
    }

    public: void fill(set<Type>& set) const
    // ************************************
    {
        GenericLocator<Type> locator = getLocator();
        while (locator.isValid()) {
            set.insert(locator.getElement());
            locator.progress();
        }
    }

    public: template<class Compare> void fill(set<Type, Compare>& set) const
    // *********************************************************************
    {
        GenericLocator<Type> locator = getLocator();
        while (locator.isValid()) {
            set.insert(locator.getElement());
            locator.progress();
        }
    }

    public: void fill(vector<Type>& vector) const
    // ******************************************
    {
        GenericLocator<Type> locator = getLocator();
        while (locator.isValid()) {
            vector.push_back(locator.getElement());
            locator.progress();
        }
    }

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("Collection<Type>");
    };

    public: virtual string _getString() const = 0;

    public: Record* _getRecord() const
    // *************************
    {
        Record* record = NULL;
        if (!IsEmpty()) {
            record = new Record(getString(this));
            unsigned n = 1;
            GenericLocator<Type> locator = getLocator();
            while (locator.isValid()) {
              string  slotName   = getString(n++);
              Type    slotRecord = locator.getElement();
              record->add(getSlot(slotName, slotRecord));
                locator.progress();
            }
        }
        return record;
    }

};



// ****************************************************************************************************
// GenericCollection declaration
// ****************************************************************************************************

template<class Type> class GenericCollection : public Collection<Type> {
// *******************************************************************

// Types
// *****

    public: typedef Collection<Type> Inherit;

// Attributes
// **********

    private: Collection<Type>* _collection;

// Constructors
// ************

    public: GenericCollection()
    // ***********************
    :    Inherit(),
        _collection(NULL)
    {
    }

    public: GenericCollection(const Collection<Type>& collection)
    // **********************************************************
    :    Inherit(),
        _collection(collection.getClone())
    {
    }

    public: GenericCollection(const GenericCollection<Type>& genericCollection)
    // ************************************************************************
    :    Inherit(),
        _collection(genericCollection.getClone())
    {
    }

    public: GenericCollection(Collection<Type>* collection)
    // *************************************************************
    // CAUTION : collection will be deleted by the GenericCollection
    // *************************************************************
    :    Inherit(),
        _collection(collection)
    {
    }

// Destructor
// **********

    public: virtual ~GenericCollection()
    // *********************************
    {
        if (_collection) delete _collection;
    }

// Operators
// *********

    public: GenericCollection& operator=(const Collection<Type>& collection)
    // *********************************************************************
    {
        if (_collection) delete _collection;
        _collection = collection.getClone();
        return *this;
    }

    public: GenericCollection& operator=(const GenericCollection& genericCollection)
    // *****************************************************************************
    {
        if (_collection) delete _collection;
        _collection = genericCollection.getClone();
        return *this;
    }

    public: GenericCollection& operator=(Collection<Type>* collection)
    // ***************************************************************
    // CAUTION : collection will be deleted by the GenericCollection
    // ***************************************************************
    {
        if (_collection) delete _collection;
        _collection = collection;
        return *this;
    }

// Accessors
// *********

    public: virtual Collection<Type>* getClone() const
    // ***********************************************
    {
        return (_collection) ? _collection->getClone() : NULL;
    }

    public: virtual Locator<Type>* getLocator() const
    // **********************************************
    {
        return (_collection) ? _collection->getLocator() : NULL;
    }

    public: virtual unsigned getSize() const
    // *************************************
    {
        return (_collection) ? _collection->getSize() : 0;
    }

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("GenericCollection");
    };

    public: virtual string _getString() const
    // **************************************
    {
        if (!_collection)
            return "<" + _getTypeName() + " unbound>";
        else
            return "<" + _getTypeName()+ " " + getString(_collection) + ">";
    }

};



// ****************************************************************************************************
// ElementCollection declaration
// ****************************************************************************************************

template<class Type> class ElementCollection : public Collection<Type> {
// *********************************************************************

    // -----------------------------------------------------------------
    // Sub-Class : "::ElementCollection::Locator".
    public: template<class ElType> class Locator : public Hurricane::Locator<ElType> {

    // Attributes
    // **********
        protected: const ElType _element;
        protected: bool _done;

    // Constructors
    // ************
        public: Locator ( const ElType  _element ) : _element(_element), _done(false) {};
        public: Locator ( const Locator &locator ) : _element(locator._element), _done(locator._done) {};

    // Accessors
    // *********
        public: virtual ElType           getElement () const { return const_cast<ElType>(_element); };
        public: virtual Locator<ElType>* getClone   () const { return new Locator(*this); };
        public: virtual bool             isValid    () const { return !_done; };
        public: virtual void             progress   ()       { _done = true; };

    // Hurricane Management
    // ********************
        public: virtual string _getString () const {
            if (!_element)
                return "<" + _TName("ElementCollection::Locator") + " unbound>";
            else
                return "<" + _TName("ElementCollection::Locator") + " " + getString(_element) + ">";
        }

    };

// Types
// *****

    public: typedef Collection<Type> Inherit;

// Attributes
// **********

    private: Type _element;

// Constructors
// ************

    public: ElementCollection()
    // ***********************
    :    Inherit(),
        _element(NULL)
    {
    }

    public: ElementCollection(const Type element)
    // **********************************************************
    :    Inherit(),
        _element(element)
    {
    }

    public: ElementCollection(const ElementCollection<Type>& elementCollection)
    // ************************************************************************
    :    Inherit(),
        _element(elementCollection._element)
    {
    }

// Accessors
// *********

    public: virtual Collection<Type>* getClone() const
    // ***********************************************
    {
        return ( new ElementCollection (*this) );
    }

    public: virtual Locator<Type>* getLocator() const
    // **********************************************
    {
        return ( new Locator<Type> (_element) );
    }

    public: virtual unsigned getSize() const
    // *************************************
    {
        return (_element) ? 1 : 0;
    }

// Others
// ******

    public: virtual string _getString() const
    // **************************************
    {
        if (!_element)
            return "<" + _TName("ElementCollection") + " unbound>";
        else
            return "<" + _TName("ElementCollection") + " " + getString(_element) + ">";
    }

};



// ****************************************************************************************************
// SubTypeCollection declaration
// ****************************************************************************************************

template<class Type, class SubType> class SubTypeCollection : public Collection<SubType> {
// *************************************************************************************

// Types
// *****

    public: typedef Collection<SubType> Inherit;

    public: class Locator : public Hurricane::Locator<SubType> {
    // *******************************************************

    // Types
    // *****

        public: typedef Hurricane::Locator<SubType> Inherit;

    // Attributes
    // **********

        private: GenericLocator<Type> _locator;

    // Constructors
    // ************

        public: Locator(const GenericCollection<Type>& collection)
        // ********************************************************
        :    Inherit(),
            _locator(collection.getLocator())
        {
            while (_locator.isValid() && !is_a<SubType>(_locator.getElement()))
                _locator.progress();
        }

        public: Locator(const GenericLocator<Type>& genericLocator)
        // ********************************************************
        :    Inherit(),
            _locator(genericLocator.getClone())
        {
            while (_locator.isValid() && !is_a<SubType>(_locator.getElement()))
                _locator.progress();
        }

    // Accessors
    // *********

        public: virtual SubType getElement() const
        // ***************************************
        {
            return (_locator.isValid()) ? (SubType)_locator.getElement() : SubType();
        }

        public: virtual Hurricane::Locator<SubType>* getClone() const
        // **********************************************************
        {
            return new Locator(_locator);
        }

        public: virtual Hurricane::Locator<SubType>* getLocator() // 21-10-03
            // *************************************************
        {
          return dynamic_cast<Hurricane::Locator<SubType>*> (
                 _locator.getLocator()->getLocator() );
        }


    // Predicates
    // **********

        public: virtual bool isValid() const
        // *********************************
        {
            return _locator.isValid();
        }

    // Updators
    // ********

        public: virtual void progress()
        // ****************************
        {
            if (_locator.isValid()) {
                do {
                    _locator.progress();
                } while (_locator.isValid() && !is_a<SubType>(_locator.getElement()));
            }
        }

    };

// Attributes
// **********

    private: GenericCollection<Type> _collection;

// Constructors
// ************

    public: SubTypeCollection()
    // ********************
    :    Inherit(),
        _collection()
    {
    }

    public: SubTypeCollection(const Collection<Type>* collection)
    // **********************************************************
    :    Inherit(),
        _collection(collection->getClone())
    {
    }

    public: SubTypeCollection(const GenericCollection<Type>& collection)
    // *****************************************************************
    :    Inherit(),
        _collection(collection)
    {
    }

    public: SubTypeCollection(const SubTypeCollection& subTypeCollection)
    // ******************************************************************
    :    Inherit(),
        _collection(subTypeCollection._collection)
    {
    }

// Operators
// *********

    public: SubTypeCollection& operator=(const SubTypeCollection& subTypeCollection)
    // *****************************************************************************
    {
        _collection = subTypeCollection._collection;
        return *this;
    }

// Accessors
// *********

    public: virtual Collection<SubType>* getClone() const
    // **************************************************
    {
        return new SubTypeCollection(_collection);
    }

    public: virtual Hurricane::Locator<SubType>* getLocator() const
    // ************************************************************
    {
        return new Locator(_collection);
    }

// Accessors
// *********

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("SubTypeCollection") + " " + getString(_collection) + ">";
    }

};



// ****************************************************************************************************
// SubSetCollection implementation
// ****************************************************************************************************

template<class Type> class SubSetCollection : public Collection<Type> {
// ******************************************************************

// Types
// *****

    public: typedef Collection<Type> Inherit;

    public: class Locator : public Hurricane::Locator<Type> {
    // ****************************************************

    // Types
    // *****

        public: typedef Hurricane::Locator<Type> Inherit;

    // Attributes
    // **********

        private: GenericLocator<Type> _locator;
        private: GenericFilter<Type> _filter;

    // Constructors
    // ************

        public: Locator(const SubSetCollection<Type>& collection, const Filter<Type>& filter)
        // **********************************************************************************
        :    Inherit(),
            _locator(collection.getLocator()),
            _filter(filter)
        {
            while (_locator.isValid() && !_filter.accept(_locator.getElement()))
                _locator.progress();
        }

        public: Locator(const Collection<Type>& collection, const Filter<Type>& filter)
        // ****************************************************************************
        :    Inherit(),
            _locator(collection.getLocator()),
            _filter(filter)
        {
            while (_locator.isValid() && !_filter.accept(_locator.getElement()))
                _locator.progress();
        }

        public: Locator(const GenericCollection<Type>& genericCollection, const Filter<Type>& filter)
        // ******************************************************************************************
        :    Inherit(),
            _locator(genericCollection.getLocator()),
            _filter(filter)
        {
            while (_locator.isValid() && !_filter.accept(_locator.getElement()))
                _locator.progress();
        }

        public: Locator(const GenericLocator<Type>& genericLocator, const Filter<Type>& filter)
        // ************************************************************************************
        :    Inherit(),
            _locator(genericLocator),
            _filter(filter)
        {
            while (_locator.isValid() && !_filter.accept(_locator.getElement()))
                _locator.progress();
        }

    // Accessors
    // *********

        public: virtual Type getElement() const
        // ************************************
        {
            return (_locator.isValid()) ? _locator.getElement() : Type();
        }

        public: virtual Hurricane::Locator<Type>* getClone() const
        // *******************************************************
        {
            return new Locator(_locator, _filter);
        }

        public: virtual Hurricane::Locator<Type>* getLocator() // 21-10-03
       // ***************************************************
        {
          return ( _locator.getLocator()->getLocator() );
        }

    // Predicates
    // **********

        public: virtual bool isValid() const
        // *********************************
        {
            return _locator.isValid();
        }

    // Updators
    // ********

        public: virtual void progress()
        // ****************************
        {
            if (_locator.isValid()) {
                do {
                    _locator.progress();
                } while (_locator.isValid() && !_filter.accept(_locator.getElement()));
            }
        }

    };

// Attributes
// **********

    private: GenericCollection<Type> _collection;
    private: GenericFilter<Type> _filter;

// Constructors
// ************

    public: SubSetCollection()
    // ***********************
    :    Inherit(),
        _collection(),
        _filter()
    {
    }

    public: SubSetCollection(const Collection<Type>& collection, const Filter<Type>& filter)
    // *************************************************************************************
    :    Inherit(),
        _collection(collection),
        _filter(filter)
    {
    }

    public: SubSetCollection(const SubSetCollection& subSetCollection)
    // ***************************************************************
    :    Inherit(),
        _collection(subSetCollection._collection),
        _filter(subSetCollection._filter)
    {
    }

// Operators
// *********

    public: SubSetCollection& operator=(const SubSetCollection& subSetCollection)
    // **************************************************************************
    {
        _collection = subSetCollection._collection;
        _filter = subSetCollection._filter;
        return *this;
    }

// Accessors
// *********

    public: virtual Collection<Type>* getClone() const
    // ***********************************************
    {
        return new SubSetCollection(_collection, _filter);
    }

    public: virtual Hurricane::Locator<Type>* getLocator() const
    // *********************************************************
    {
        return new Locator(_collection, _filter);
    }

// Accessors
// *********

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("SubSetCollection") + " " + getString(_collection) + ">";
    }

};



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************



// ****************************************************************************************************
// Macros declaration
// ****************************************************************************************************

#define end_for\
/**************/\
        }\
    }

#define for_each_object(Type, element, collection)\
/*************************************************/\
{\
    GenericLocator<Type> _locator = collection.getLocator();\
    while (_locator.isValid()) {\
        Type element = _locator.getElement();\
        _locator.progress();

#define for_each_element(Type, element, collection)\
/*************************************************/\
{\
    ElementCollection<Type>::Locator<Type>* _locator = collection.getLocator();\
    while (_locator->isValid()) {\
        Type element = _locator->getElement();\
        _locator->progress();


template<typename T>
    class IsNestedSlotAdapter<const Hurricane::GenericCollection<T> > {
      public:
        enum { True=1, False=0 };
    };



} // End of Hurricane namespace.


#include "MultisetCollection.h"
#include "SetCollection.h"
#include "MapCollection.h"
#include "ListCollection.h"
#include "VectorCollection.h"

#endif // HURRICANE_COLLECTION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
