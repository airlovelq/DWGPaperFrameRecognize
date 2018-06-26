// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "ObjInterAnalyze.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
void process()
{
	CString strFolder = _T("");
	LPMALLOC lpMalloc; 
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return; 
	TCHAR szDisplayName[_MAX_PATH];
	TCHAR szBuffer[_MAX_PATH]; 

	BROWSEINFO browseInfo; 
	browseInfo.hwndOwner = acedGetAcadFrame()->GetSafeHwnd(); 
	browseInfo.pidlRoot = NULL; 
	browseInfo.pszDisplayName = szDisplayName; 
	browseInfo.lpszTitle = _T("选择一个文件夹");
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browseInfo.lpfn = NULL; 
	browseInfo.lParam = 0; 
	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer)) 
		{  // At this point, szBuffer contains the path the user chose. 
			if (szBuffer[0] != _T('\0')) 
			    // We have a path in szBuffer! Return it. 
			    strFolder = szBuffer;  
		} 
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release();
	} 
	if ( strFolder.GetLength() <= 0 )
		return;

	CObjInterAnalyze p;
	p.Analyze();
	vector<SFrame> arr = p.GetArrFrame();
	int size = arr.at(0).arrIds.length();
	for ( int j=0 ; j<arr.size() ; j++ )
	{
		acedCommand(RTSTR,_T("zoom"),RTSTR,_T("W"),RT3DPOINT,asDblArray(arr.at(j).ptMin),RT3DPOINT,asDblArray(arr.at(j).ptMax),RTNONE);
		TCHAR kw[20];
		acedGetString(0,_T("\n是否为图框:"),kw);
		CString str;
		CString sCurPath;
        AcApDocument* pDoc = acDocManager->curDocument();
        sCurPath = pDoc->fileName();
		sCurPath = sCurPath.Right(sCurPath.GetLength() - sCurPath.ReverseFind(_T('\\')) - 1 );
		sCurPath = sCurPath.Left(sCurPath.ReverseFind(_T('.')) );
		str.Format(_T("%s\\%s-%s-%d.jpg"),strFolder,kw,sCurPath,j+1);
		ads_name ssname;
		for ( int i=0 ; i<arr.at(j).arrIds.length() ; i++ )
	    {
			ads_name ename;
			acdbGetAdsName(ename,arr.at(j).arrIds.at(i));
			acedSSAdd(ename,ssname,ssname);
		    //AcDbObjectPointer<AcDbEntity> pEnt(arr.at(j).arrIds.at(i),AcDb::kForRead);
		    //pEnt->highlight();
	    }
		//acedCommand(RTSTR,_T("wmfout"),RTSTR,str,RTENAME,ssname,RTNONE);
		acedCommand(RTSTR, _T("-PLOT")		
						, RTSTR, _T("Y")					// 详细配置
						, RTSTR, _T("")						// 布局名称
						, RTSTR, _T("PublishToWeb JPG.pc3")					// 输出设备
						, RTSTR, _T("VGA (640.00 x 480.00 像素)")					// 图纸尺寸
						//, RTSTR, szUnit						// 单位（英寸I 毫米M）
						, RTSTR, ((arr.at(j).ptMax.x - arr.at(j).ptMin.x) > (arr.at(j).ptMax.y - arr.at(j).ptMin.y))?_T("L"):_T("P")				// 方向（纵向P 横向L）
						, RTSTR, _T("N")					// 是否上下颠倒
						, RTSTR, _T("W")					// 打印区域
						, RTPOINT,asDblArray(arr.at(j).ptMin)		// 左下角点
						, RTPOINT, asDblArray(arr.at(j).ptMax)	// 右上角点
						, RTSTR, _T("F")					// 打印比例或布满
						, RTSTR, _T("C")					// 打印便宜(x,y)或居中(C)
						, RTSTR, _T("Y")					// 是否按样式打印
						, RTSTR, _T("acad.ctb")				// 样式表名称
						, RTSTR, _T("Y")					// 是否打印线宽
						, RTSTR, _T("A")					// 着色打印设置（显示A
						//, RTSTR, _T("Y")					// 打印到文件
						, RTSTR, str				// 输出文件路径
						, RTSTR, _T("N")					// 是否保存打印页面设置
						, RTSTR, _T("Y")					// 是否打印
						, RTNONE);
		acedSSFree(ssname);
		/*
		for ( int i=0 ; i<arr.at(j-1).arrIds.length() ; i++ )
		{
			AcDbObjectPointer<AcDbEntity> pEnt(arr.at(j-1).arrIds.at(i),AcDb::kForRead);
		    pEnt->unhighlight();
		}
	    for ( int i=0 ; i<arr.at(j).arrIds.length() ; i++ )
	    {
		    AcDbObjectPointer<AcDbEntity> pEnt(arr.at(j).arrIds.at(i),AcDb::kForRead);
		    pEnt->highlight();
	    }
		TCHAR kw[20];
		acedGetKword(_T("输入"),kw);*/
	}
}
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CPaperFrameRecognizeApp : public AcRxArxApp {

public:
	CPaperFrameRecognizeApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CPaperFrameRecognizeApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CPaperFrameRecognizeApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void MyGroupMyCommand () {
		// Put your command code here
		process();
	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CPaperFrameRecognizeApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void MyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CPaperFrameRecognizeApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void MyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CPaperFrameRecognizeApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CPaperFrameRecognizeApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CPaperFrameRecognizeApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CPaperFrameRecognizeApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CPaperFrameRecognizeApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CPaperFrameRecognizeApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CPaperFrameRecognizeApp, MyLispFunction, false)

