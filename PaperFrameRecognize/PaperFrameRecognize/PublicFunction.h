#pragma once
namespace Pub
{
	AcDbObjectId GetModalSpaceId ( AcDbDatabase * pDatabase );

	Acad::ErrorStatus GetInsertGeomExtents ( const AcDbEntity * pInsert , AcDbExtents & extent );
};