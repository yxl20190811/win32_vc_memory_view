
// MemoryViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MemoryView.h"
#include "MemoryViewDlg.h"
#include "afxdialogex.h"
#include <Tlhelp32.h>
#include <string.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

const char* InjectDllName = "hook.dll";
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMemoryViewDlg 对话框




CMemoryViewDlg::CMemoryViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMemoryViewDlg::IDD, pParent)
{
	m_DmpOffset = NULL;
	m_FileNameOffset = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemoryViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lst);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonStart);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonReport);
}

BEGIN_MESSAGE_MAP(CMemoryViewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CMemoryViewDlg::OnBnClickedButton_Fresh)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMemoryViewDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMemoryViewDlg::OnBnClickedButton_Report)
	ON_BN_CLICKED(IDC_BUTTON1, &CMemoryViewDlg::OnBnClickedButton_StartMonitor)
END_MESSAGE_MAP()


// CMemoryViewDlg 消息处理程序

BOOL CMemoryViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	m_lst.SetExtendedStyle(m_lst.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT );
	m_lst.InsertColumn(0, "进程名称", 0, 100);
	m_lst.InsertColumn(1, "进程ID", 0, 100);
	m_lst.InsertColumn(2, "进程路径", 0, 1000);
	m_lst.InsertColumn(3, "hook地址", 0, 500);
	OnBnClickedButton_Fresh();
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMemoryViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMemoryViewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMemoryViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void  CMemoryViewDlg::InsertProcess2Lst(DWORD ProcessId)
{
	HANDLE hProcess = ::OpenProcess(
		PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if(NULL != hProcess)
	{
		char FileName[1024+MAX_PATH];
		if(0 != GetModuleFileNameEx(hProcess, NULL, FileName, sizeof(FileName)))
		{
			char* pos = strrchr(FileName, '\\');
			int count = m_lst.GetItemCount();
			m_lst.InsertItem(count, pos + 1);
			char buf[100];
			sprintf(buf, "%u", ProcessId);
			m_lst.SetItemText(count, 1, buf);
			m_lst.SetItemText(count, 2, FileName);
			++count;
		}
		::CloseHandle(hProcess);
	}
}

void CMemoryViewDlg::OnBnClickedButton_Fresh()
{
	m_lst.DeleteAllItems();
	// TODO: 在此添加控件通知处理程序代码
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;
	DWORD cProcesses = cbNeeded / sizeof(DWORD);
	for(DWORD i = 0; i < cProcesses; ++i)
	{
		InsertProcess2Lst(aProcesses[i]);
	}

	/*

	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(pe32);
	HANDLE hProcessSnap =::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	BOOL bMore=::Process32First(hProcessSnap,&pe32);
	int count = 0;
	while(bMore)
	{
		if(0 != pe32.th32ProcessID)
		{
			InsertProcess2Lst(aProcesses[i]);
		}
		bMore=::Process32Next(hProcessSnap,&pe32);
	}
	*/
	m_lst.SetItemState(0, 
	 LVIS_SELECTED | LVIS_FOCUSED,
	 LVIS_SELECTED | LVIS_FOCUSED);
}

HMODULE FindModule(HANDLE hProcess, const char* FindModuleName)
{
	HMODULE hMods[1024];
    DWORD cbNeeded;
	if(0==EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		return NULL;
	}
	
	bool IsFind = false;
	for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
    {
		char szModName[MAX_PATH];
		 if (!GetModuleFileNameEx( hProcess, hMods[i], szModName,sizeof(szModName)))
		 {
			 continue;
		 }
		 if(0 != strstr(szModName, FindModuleName))
		 {
			 return hMods[i];
		 }
	}
	return NULL;
}

void CMemoryViewDlg::OnSelectChanged(int index)
{
	m_ButtonStart.EnableWindow(FALSE);
	m_ButtonReport.EnableWindow(FALSE);


	char buf[1024];
	m_lst.GetItemText(index, 1, buf, sizeof(buf));
	DWORD ProcessId = atoi(buf);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, ProcessId );
	if(NULL == hProcess)
	{
		return;
	}
	HMODULE hModule = FindModule(hProcess, InjectDllName);
	::CloseHandle(hProcess);
	if(NULL == hModule)
	{
		m_ButtonStart.EnableWindow(TRUE);
	}
	else
	{
		m_ButtonReport.EnableWindow(TRUE);
	}
}
void CMemoryViewDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
	if(0 == pNMLV->uNewState)
	{
		return;
	}
	OnSelectChanged(pNMLV->iItem);
	
}


void CMemoryViewDlg::OnBnClickedButton_Report()
{
	int index = m_lst.GetSelectionMark();
	char buf[1024];
	m_lst.GetItemText(index, 1, buf, sizeof(buf));
	DWORD ProcessId = atoi(buf);
	//寻找进程下是否已经存在hook
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, ProcessId );
	if(NULL == hProcess)
	{
		::MessageBoxA(this->GetSafeHwnd(), "OpenProcess失败", "错误",MB_ICONERROR );
		return;
	}
	HMODULE hModule = FindModule(hProcess, InjectDllName);
	if(NULL == hModule)
	{
		::MessageBoxA(this->GetSafeHwnd(), "hook失败", "错误",MB_ICONERROR );
		::CloseHandle(hProcess);
		return;
	}
	if(NULL == m_DmpOffset || NULL == m_FileNameOffset)
	{
		char FileName[1024+MAX_PATH];
		::GetModuleFileNameA(NULL, FileName, sizeof(FileName));
		char* pos = strrchr(FileName, '\\');
		sprintf(pos, "\\%s", InjectDllName);
		pos = strrchr(FileName, '.');
		strcpy(pos, ".data");
		FILE* fp = fopen(FileName, "rb");
		if(NULL == fp)
		{
			::MessageBoxA(this->GetSafeHwnd(), "读取hook配置文件失败", "错误",MB_ICONERROR );
			::CloseHandle(hProcess);
			return;
		}
		fscanf(fp, "%I64u", &m_FileNameOffset);
		fscanf(fp, "%I64u", &m_DmpOffset);
		fclose(fp);
	}
	if(NULL == m_DmpOffset || NULL == m_FileNameOffset)
	{
		::MessageBoxA(this->GetSafeHwnd(), "读取hook失败", "错误",MB_ICONERROR );
		::CloseHandle(hProcess);
		return;
	}
	{
		char FileName[1024*100];
		m_lst.GetItemText(index, 2, FileName, sizeof(FileName));
		char* pos = strrchr(FileName, '.');
		sprintf(pos, ".report");
		long long fun = (long long)hModule+m_DmpOffset;
		long long param = (long long)hModule+m_FileNameOffset;
		SIZE_T NumberOfBytesWritten = 0;
		WriteProcessMemory(hProcess, 
			(LPVOID)(param),
			FileName,
			strlen(FileName)+1, 
			&NumberOfBytesWritten);
		DWORD ThreadId = 0;
		HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)(fun),
			(LPVOID)param,
			0, &ThreadId);
		::WaitForSingleObject(hThread, INFINITE);
		::CloseHandle(hThread);
		char buf[1024*100];
		sprintf(buf, "成功打印报告[%s]", FileName);
		::MessageBoxA(this->GetSafeHwnd(), buf, "成功", MB_OK);
	}
	::CloseHandle(hProcess);
	return;
}


void CMemoryViewDlg::OnBnClickedButton_StartMonitor()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_lst.GetSelectionMark();
	char buf[1024];
	m_lst.GetItemText(index, 1, buf, sizeof(buf));
	DWORD ProcessId = atoi(buf);
	//打开进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, ProcessId );
	if(NULL == hProcess)
	{
		::MessageBoxA(this->GetSafeHwnd(), "OpenProcess失败", "错误",MB_ICONERROR );
		return;
	}
	//分配内存
	char FileName[1024+MAX_PATH];
	{
		::GetModuleFileNameA(NULL, FileName, sizeof(FileName));
		char* pos = strrchr(FileName, '\\');
		sprintf(pos, "\\%s", InjectDllName);
	}
	int FileNameLen = strlen(FileName)+1;
	void* FileName_Addr = ::VirtualAllocEx(
		hProcess, NULL, 
		FileNameLen, MEM_COMMIT, PAGE_READWRITE );
	SIZE_T NumberOfBytesWritten = 0;
	WriteProcessMemory(hProcess, 
		(LPVOID)(FileName_Addr),
		FileName,
		FileNameLen, 
		&NumberOfBytesWritten);
	DWORD ThreadId = 0;

	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)(::GetProcAddress(::LoadLibraryA("kernel32.dll"),"LoadLibraryA")),
		(LPVOID)FileName_Addr,
		0, &ThreadId);
	::WaitForSingleObject(hThread, INFINITE);
	::CloseHandle(hThread);
	HMODULE hModule = FindModule(hProcess, InjectDllName);
	if(NULL == hModule)
	{
		::MessageBoxA(this->GetSafeHwnd(), "hook失败", "错误",MB_ICONERROR );
		::CloseHandle(hProcess);
		return;
	}
	else
	{
		::MessageBoxA(this->GetSafeHwnd(), "成功开启监视", "成功",MB_OK );
		::CloseHandle(hProcess);
		m_ButtonStart.EnableWindow(FALSE);
		m_ButtonReport.EnableWindow(TRUE);
		return;
	}
}
