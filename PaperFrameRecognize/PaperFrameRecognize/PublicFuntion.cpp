#include "StdAfx.h"
#include "PublicFunction.h"
namespace Pub
{
    AcDbObjectId GetModalSpaceId ( AcDbDatabase * pDatabase )
    {
	    Acad::ErrorStatus es ;
	    if ( pDatabase == NULL )
		    pDatabase = acdbHostApplicationServices ()->workingDatabase () ;
	    if ( pDatabase == NULL )
	    {
		    return AcDbObjectId::kNull ;
	    }

	    AcDbBlockTable * pBlockTable = NULL ;
	    es = pDatabase->getBlockTable ( pBlockTable , AcDb::kForRead ) ;	// 打开块表
	    if ( es != Acad::eOk || pBlockTable == NULL )
	    {
			return AcDbObjectId::kNull ;
	    }
	    pBlockTable->close () ;

	    AcDbObjectId idMs = AcDbObjectId::kNull ;
	    es = pBlockTable->getAt ( ACDB_MODEL_SPACE , idMs ) ;
	    if ( es != Acad::eOk )						// 打开模型空间
	    {
		    return AcDbObjectId::kNull ;
	    }
	    return idMs ;
    }

    Acad::ErrorStatus GetInsertGeomExtents ( const AcDbEntity * pInsert , AcDbExtents & extent )
    {
	    if ( pInsert == NULL )
		    return eInvalidInput ;
	    AcDbVoidPtrArray aryExploded ;
	    Acad::ErrorStatus es = pInsert->explode ( aryExploded ) ;
	    if ( es != Acad::eOk )
	    {
		    return pInsert->getGeomExtents ( extent ) ;
	    }

	    extent.set ( AcGePoint3d ( 0 , 0 , 0 ) , AcGePoint3d ( 0 , 0 , 0 ) ) ;
	    BOOL bFirst = TRUE ;
		for ( int i = 0 ; i < aryExploded.length() ; i ++ )
	    {
		    AcDbEntity * pEntity = ( AcDbEntity * ) aryExploded.at ( i ) ;
		    if ( pEntity->isKindOf ( AcDbText::desc () ) || pEntity->isKindOf ( AcDbMText::desc () ) )
		    {
			    delete pEntity ;
			    continue ;
		    }

		    if ( pEntity->isKindOf ( AcDbPolyline::desc () ) )
		    {
			    AcDbPolyline * pPoly = ( AcDbPolyline * ) pEntity ;
			    pPoly->setConstantWidth ( 0.0 ) ;
		    }

		    AcDbExtents extNow ;
		    pEntity->getGeomExtents ( extNow ) ;
		    if ( bFirst )
		    {
			    extent = extNow ;
			    bFirst = FALSE ;
		    }
		    else
		    {
			    AcGePoint3d ptMin , ptMax ;
			    ptMin.x = min ( extent.minPoint ().x , extNow.minPoint ().x ) ;
			    ptMin.y = min ( extent.minPoint ().y , extNow.minPoint ().y ) ;
			    ptMax.x = max ( extent.maxPoint ().x , extNow.maxPoint ().x ) ;
			    ptMax.y = max ( extent.maxPoint ().y , extNow.maxPoint ().y ) ;
			    extent.set ( ptMin , ptMax ) ;
		    }
		    delete pEntity ;
	    }
	    return eOk ;
    }
}