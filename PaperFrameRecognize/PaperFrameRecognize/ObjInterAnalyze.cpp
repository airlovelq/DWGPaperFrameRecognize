#include "stdafx.h"
#include "ObjInterAnalyze.h"
#include "PublicFunction.h"

CObjInterAnalyze::CObjInterAnalyze(AcDbDatabase *pDb)
	:m_pDb(pDb)
{
	m_arrFrame.empty();
}


CObjInterAnalyze::~CObjInterAnalyze(void)
{
}

bool CObjInterAnalyze::Interfere( SFrame &src, SFrame &dst )//判断两实体是否相交并重新设置最大包容框
{
	const double nError = 0 ;				// 线宽导致的容差增加,未测试出必要性，置零不用

	bool bX_NoInter = (( src.ptMin.x < ( dst.ptMin.x + nError ) && src.ptMax.x < ( dst.ptMin.x + nError ) )|| 
		( src.ptMin.x > ( dst.ptMax.x - nError ) && src.ptMax.x > ( dst.ptMax.x - nError ) )) ;
	bool bY_NoInter = (( src.ptMin.y < ( dst.ptMin.y + nError ) && src.ptMax.y < ( dst.ptMin.y + nError ) )|| 
		( src.ptMin.y > ( dst.ptMax.y - nError ) && src.ptMax.y > ( dst.ptMax.y - nError ) )) ;

	if ( bX_NoInter || bY_NoInter )
	{
		return false ;
	}
	if ( !bX_NoInter )
	{
		dst.ptMin.x = min ( src.ptMin.x , dst.ptMin.x ) ;
		dst.ptMax.x = max ( src.ptMax.x , dst.ptMax.x ) ;
		
	}
	if ( !bY_NoInter )
	{
		dst.ptMin.y = min ( src.ptMin.y , dst.ptMin.y ) ;
		dst.ptMax.y = max ( src.ptMax.y , dst.ptMax.y ) ;
		
	}
	dst.arrIds.append(src.arrIds);
	return true ;
}

bool CObjInterAnalyze::Merge ( SFrame &dst )//不断判断输入框架是否与其他框架干涉，不断取得最大包容实体
{
	size_t nCount = m_arrFrame.size () ;
	for ( size_t i = 0 ; i < nCount ; i ++ )
	{
		SFrame &dbCur = m_arrFrame[i] ;

		bool bInter = Interfere ( dbCur , dst ) ;
		if ( bInter )
		{
			m_arrFrame.erase ( m_arrFrame.begin () + i ) ;
			return Merge ( dst ) ;
		}
	}
	m_arrFrame.push_back ( dst ) ;
	return false ;
}

vector < SFrame > CObjInterAnalyze::GetArrFrame() const
{
	return m_arrFrame;
}

void CObjInterAnalyze::Analyze()
{
	AcDbObjectId idSpace = Pub::GetModalSpaceId(m_pDb);
	if ( AcDbObjectId::kNull == idSpace )
		return;
	Acad::ErrorStatus es;
	AcDbObjectPointer<AcDbBlockTableRecord> pModelSpace(idSpace,AcDb::kForRead);
	if ( pModelSpace.openStatus() != Acad::eOk )
		return;
	AcDbBlockTableRecordIterator *pIter = NULL;
	es = pModelSpace->newIterator(pIter);
	if ( es != Acad::eOk )
		return;
	for ( pIter->start() ; !pIter->done() ; pIter->step() )
	{
		AcDbObjectId id;
		es = pIter->getEntityId(id);
		if ( AcDbObjectId::kNull == id )
			continue;
		AcDbObjectPointer<AcDbEntity> pEnt(id,AcDb::kForRead);
		if ( pEnt.openStatus() != Acad::eOk )
			continue;
		AcDbExtents ex;
		if ( Acad::eOk != Pub::GetInsertGeomExtents(pEnt,ex))
			continue;
		if (( fabs( ex.maxPoint().x - ex.minPoint().x )<1e-3 ) && ( fabs( ex.maxPoint().y - ex.minPoint().y )<1e-3 ))
			continue;
		SFrame atom;
		atom.ptMax = ex.maxPoint();
		atom.ptMin = ex.minPoint();
		atom.arrIds.append(id);
		Merge(atom);
	}
	delete pIter;
}


CObjInterAnalyze2::CObjInterAnalyze2()
{
}

CObjInterAnalyze2::~CObjInterAnalyze2()
{

}

bool CObjInterAnalyze2::Interfere( SFrame &src, SFrame &dst )
{
	for ( int i=0 ; i<src.arrIds.length() ; i ++ )
	{
		AcDbObjectId id = src.arrIds.at(i);
		AcDbObjectPointer<AcDbEntity> pEnt(id,AcDb::kForRead);
		if ( pEnt.openStatus() != Acad::eOk )
			continue;
		for ( int j=0 ; j<dst.arrIds.length() ; j ++ )
		{
			AcDbObjectId idd = dst.arrIds.at(j);
			AcDbObjectPointer<AcDbEntity> pEntd(idd,AcDb::kForRead);
			if ( pEntd.openStatus() != Acad::eOk )
			    continue;
			AcGePoint3dArray pts;
			if ( pEnt->intersectWith(pEntd,AcDb::kOnBothOperands,pts) == Acad::eOk )
			{
				if ( pts.length() > 0 )
				{
					dst.arrIds.append(src.arrIds);
					return true;
				}
			}
		}
	}
	return false;
}