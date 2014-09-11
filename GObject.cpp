#include "GGameDemoHeader.h"
#include "GObject.h"

int gCount = 0;
GObject::GObject ( void )
{
    mNodeName.clear();
    mNodeName.Format ( "GObject%d", gCount );

    gCount++;
}


GObject::~GObject ( void )
{
}

void GObject::RegisterAll()
{
    RegisterProperty ( "GObject", "mNodeName", mNodeName );
    //__RegisterProperty(mEObjectName);
}

void GObject::RegisterProperty ( GObject* obj )
{
    assert ( obj );
    if ( !obj->IsRegisterAll() )
    {
        obj->RegisterAll();
    }
    const CategoryPropertyMap& otherPropMap = obj->GetPropertyMap();
    CategoryPropertyMap::const_iterator ibegin = otherPropMap.begin();
    CategoryPropertyMap::const_iterator iend = otherPropMap.end();
    for ( ; ibegin != iend; ++ibegin )
    {
        PropertyMap* myPropMap = 0;
        if ( mOption.Get ( ibegin->first, myPropMap ) )
        {
            assert ( 0 );
        }
        else
        {
            myPropMap = new PropertyMap;
            mOption.Insert ( ibegin->first, myPropMap );
        }
        const PropertyMap* propMap = ibegin->second;
        for ( PropertyMap::const_iterator walk = propMap->begin();
                walk != propMap->end(); ++walk )
        {
            EPropertyVar* evar = new EPropertyVar;
            evar->Ptr = walk->second->Ptr;
            evar->CategoryName = walk->second->CategoryName;
            evar->QVar = walk->second->QVar;
            myPropMap->Insert ( walk->first, evar );
        }
    }
    //mOption.insert ( ibegin, iend );
}

void GObject::UnRegisterAll()
{
    CategoryPropertyMap::iterator walk = mOption.begin();
    CategoryPropertyMap::iterator end = mOption.end();
    for ( ; walk != end; ++walk )
    {
        PropertyMap* propMap = walk->second;
        CXSafeDeleteMap2 ( *propMap );
    }
    mOption.clear();
}

void GObject::OnPropertyChange( void* pre,void* changed )
{
	if ( pre == &mNodeName )
	{
		//String changeName(*((String*)changed));
		//EditorEvent event;
		//event.mType = eSceneToEditor_AlterName;
		//event.mArgs.push_back ( mNodeName );
		//event.mArgs.push_back ( changeName );
		//Notify ( event );
	}
}

EPropertyVar::EPropertyVar()
    : Ptr ( 0 )
{

}
static const int BufferSize = 1024;
char gCharBuffer[BufferSize];
void EPropertyVar::ToString ( std::string& str )
{
   if (QVar)
   {
	   QVar->toString(str);
   }
}
