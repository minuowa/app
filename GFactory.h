#pragma once
template<typename T>
class GFactory
{
public:
    typedef T* ( *ObjCrator ) ();
    typedef CXMap<CharString, ObjCrator> ObjCreatorMap;

    //GFactory ( void );
    //~GFactory ( void );
    T*	Create ( const char* typeName )
    {
        ObjCrator creator = GetCreator ( typeName );
        CXASSERT_RETURN_FALSE ( creator );
        return creator();
    }
    void RegisterCreator ( const char* typeName, ObjCrator creator )
    {
        if ( mCreatorMap.find ( typeName ) == mCreatorMap.end() )
        {
            mCreatorMap.Insert ( typeName, creator );
        }
        else
        {
            CXASSERT ( 0 );
        }
    }
    ObjCrator GetCreator ( const char* typeName )
    {
        ObjCrator creator = 0;
        CXASSERT_RETURN_FALSE ( mCreatorMap.Get ( typeName, creator ) );
        return creator;
    }
    const ObjCreatorMap& GetCreators() const
    {
        return mCreatorMap;
    }
protected:
    ObjCreatorMap	mCreatorMap;
};

#define __RegisterCreator(className)\
	RegisterCreator(#className,className::CreateNode);


#define DeclareNodeCreator(className) \
	public:\
	static GNode* CreateNode() \
	{\
		return new className();\
	}

#define DeclareEditorType(className) \
	DeclareCategoryName(className)\
	DeclareNodeCreator(className)\
 
