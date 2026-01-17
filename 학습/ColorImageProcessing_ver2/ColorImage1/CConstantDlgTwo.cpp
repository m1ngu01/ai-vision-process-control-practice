// CConstantDlgTwo.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImage1.h"
#include "afxdialogex.h"
#include "CConstantDlgTwo.h"


// CConstantDlgTwo 대화 상자

IMPLEMENT_DYNAMIC(CConstantDlgTwo, CDialog)

CConstantDlgTwo::CConstantDlgTwo(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent)
	, m_valueX(0)
	, m_valueY(0)
{

}

CConstantDlgTwo::~CConstantDlgTwo()
{
}

void CConstantDlgTwo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_valueX);
	DDX_Text(pDX, IDC_EDIT2, m_valueY);
}


BEGIN_MESSAGE_MAP(CConstantDlgTwo, CDialog)
END_MESSAGE_MAP()


// CConstantDlgTwo 메시지 처리기
