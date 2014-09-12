#pragma once
#include "XString.h"
#include "XMap.h"
#include "XMultimap.h"

typedef String EPropertyString;

#define DeclareCategoryName(className)\
		public:\
		virtual const char* CategoryName() const\
{\
	return #className;\
}\
 
struct  EPropertyVar
{
    void* Ptr;
    EPropertyString CategoryName;
    CXProp* QVar;
    void ToString ( std::string& str );
    EPropertyVar();
};
typedef CXMap<EPropertyString, EPropertyVar*> PropertyMap;
typedef CXMap<EPropertyString, PropertyMap*> CategoryPropertyMap;
class GObject:public CXCallBack
{
    DeclareCategoryName ( GObject );
public:
    GObject ( void );
    ~GObject ( void );
public:
    template<typename T>
    void RegisterProperty ( const char* categoryName, const char* propName, const T& var )
    {
        PropertyMap* propMap = 0;
        if ( !mOption.Get ( categoryName, propMap ) )
        {
            propMap = new PropertyMap;
            mOption.Insert ( categoryName, propMap );
        }

        EPropertyVar* evar = 0;
        if ( !propMap->Get ( propName, evar ) )
        {
            EPropertyVar* evar = new EPropertyVar;
            evar->QVar = new CXPropEntity<T> ( (T*)&var,false);
            evar->Ptr = ( void* ) &var;
            propMap->Insert ( propName, evar );
            evar->CategoryName = categoryName;
        }
        else
        {
            assert ( 0 && "already exist property" );
        }
    }
    template<>
    void RegisterProperty ( const char* categoryName, const char* propName, const EPropertyString& var )
    {
        PropertyMap* propMap = 0;
        if ( !mOption.Get ( categoryName, propMap ) )
        {
            propMap = new PropertyMap;
            mOption.Insert ( categoryName, propMap );
        }

        EPropertyVar* evar = 0;
        if ( !propMap->Get ( propName, evar ) )
        {
            EPropertyVar* evar = new EPropertyVar;
            evar->QVar = new CXPropEntity<String> ( new  String(var.c_str() ),false);
            evar->Ptr = ( void* ) &var;
            propMap->Insert ( propName, evar );
            evar->CategoryName = categoryName;
        }
        else
        {
            assert ( 0 && "already exist property" );
        }
    }
    virtual void RegisterAll();
	void UnRegisterAll();
    void RegisterProperty ( GObject* obj );
    const CategoryPropertyMap& GetPropertyMap() const
    {
        return mOption;
    }
    CategoryPropertyMap& GetPropertyMap()
    {
        return mOption;
    }
	virtual void OnPropertyChange(void* pre,void* changed);
protected:
    EPropertyString	mNodeName;
    CategoryPropertyMap mOption;
public:
    bool IsRegisterAll()
    {
        return mOption.size();
    }

	inline const CChar* GetObjectName() const
	{
		return mNodeName.c_str();
	}
};


#define __RegisterProperty(mem) RegisterProperty(CategoryName(),#mem,mem)
