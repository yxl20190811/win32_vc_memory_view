
// MemoryViewDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMemoryViewDlg �Ի���
class CMemoryViewDlg : public CDialogEx
{
// ����
public:
	CMemoryViewDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MEMORYVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
