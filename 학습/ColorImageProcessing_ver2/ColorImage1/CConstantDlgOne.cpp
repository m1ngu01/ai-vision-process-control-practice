// CConstantDlgOne.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImage1.h"
#include "afxdialogex.h"
#include "CConstantDlgOne.h"


// CConstantDlgOne 대화 상자

IMPLEMENT_DYNAMIC(CConstantDlgOne, CDialog)

CConstantDlgOne::CConstantDlgOne(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_value(0)
{

}

CConstantDlgOne::~CConstantDlgOne()
{
}

void CConstantDlgOne::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_value);
}


BEGIN_MESSAGE_MAP(CConstantDlgOne, CDialog)
END_MESSAGE_MAP()


// CConstantDlgOne 메시지 처리기
