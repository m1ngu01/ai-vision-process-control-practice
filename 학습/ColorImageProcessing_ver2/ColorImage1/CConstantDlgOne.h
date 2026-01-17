#pragma once
#include "afxdialogex.h"


// CConstantDlgOne 대화 상자

class CConstantDlgOne : public CDialog
{
	DECLARE_DYNAMIC(CConstantDlgOne)

public:
	CConstantDlgOne(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConstantDlgOne();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	double m_value;
};
