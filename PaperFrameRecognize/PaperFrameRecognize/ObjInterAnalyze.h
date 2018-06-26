#pragma once

struct SFrame
{
    AcGePoint3d ptMax;
	AcGePoint3d ptMin;
	AcDbObjectIdArray arrIds;
};

class CObjInterAnalyze
{
public:
	CObjInterAnalyze(AcDbDatabase *pDb = NULL);
	virtual ~CObjInterAnalyze(void);

protected:
	AcDbDatabase *m_pDb;
	vector < SFrame > m_arrFrame;
protected:
	virtual bool Interfere( SFrame &src, SFrame &dst );    //判断两实体是否相交并重新设置最大包容框,并合并ids
	bool Merge ( SFrame &dst );                    //不断判断输入框架是否与其他框架干涉，不断取得最大包容实体

public:
	vector < SFrame > GetArrFrame() const;
	void Analyze();
};

class CObjInterAnalyze2 : public CObjInterAnalyze
{
public:
	CObjInterAnalyze2 ();
	~CObjInterAnalyze2 ();

protected:
	virtual bool Interfere( SFrame &src, SFrame &dst );    //判断两实体是否相交并重新设置最大包容框,并合并ids
};

