
// MemoryViewDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMemoryViewDlg 对话框
class CMemoryViewDlg : public CDialogEx
{
// 构造
public:
	CMemoryViewDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MEMORYVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lst;
	CButton m_ButtonStart;
	CButton m_ButtonReport;
	afx_msg void OnBnClickedButton_Fresh();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void OnSelectChanged(int index);
	void  InsertProcess2Lst(DWORD ProcessId);
	afx_msg void OnBnClickedButton_Report();
private:
	long long m_DmpOffset;
	long long m_FileNameOffset;
public:
	afx_msg void OnBnClickedButton_StartMonitor();
};
