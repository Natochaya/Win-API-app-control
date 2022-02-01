#include "main.h"

struct ForMenuItemClick
{
	int IDOfMIOfProg;
	UINT IDOfMIOfCalc;
};

struct ForButtonClick
{
	int IDOfMIButOfProg;
	HWND hButton;
};

static PROCESS_INFORMATION PICalc;
static HWND hWndOfCalc;
static HMENU hMenuOfCalc;
static char NamesOfDoughWnds[5256];
static int SSmesForButtons, SmesForButtons, IDOfMenuItem, CounterOfMI, CounterOfBut, FFB, NumOfButMI;
static ForMenuItemClick* ArForMIClick;
static ForButtonClick* ArForButClick;

//Главная функция 
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, 
	LPSTR lpszCmdParam,int nCmdShow) 
{ 
	hInst=hInstance; 
	WNDCLASS WndClass; 
	MSG Msg; 
	char ClassName[]="MYPROJECT"; 
	HWND hWnd; 
	WndClass.style=CS_HREDRAW | CS_VREDRAW; 
	WndClass.lpfnWndProc=WIN32PROC; 
	WndClass.cbClsExtra=0; 
	WndClass.cbWndExtra=0; 
	WndClass.hInstance=hInstance; 
	WndClass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW); 
	WndClass.hbrBackground=(HBRUSH)NULL; 
	WndClass.lpszMenuName=NULL; 
	WndClass.lpszClassName=ClassName; 
	if (!RegisterClass(&WndClass)) 
	{ 
		MessageBox(NULL,"Cannot register class","Error",MB_OK | MB_ICONERROR); 
		return 0; 
	} 
	hWnd=CreateWindowEx(0, ClassName,"Дочерние окна и управление «чужим» приложением", 
		WS_OVERLAPPEDWINDOW, 
		300,100,420,300, 
		NULL,NULL,hInstance,NULL); 
	if (hWnd==NULL) 
	{ 
		MessageBox(NULL,"Cannot create window","Error",MB_OK | MB_ICONERROR); 
		return 0; 
	} 
	ShowWindow(hWnd,nCmdShow); 
	UpdateWindow(hWnd); 
	MSG msg; 
	while (1) 
	{ 
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
		{ 
			if (GetMessage(&msg, NULL, 0, 0) ) 
			{ 
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			} 
			else 
			{ 
				return TRUE; 
			} 
		} 
	} 
	return Msg.wParam; 
} 

//функция обработки сообщения диалогового окна
LRESULT CALLBACK WIN32DIAL(HWND hWnd,UINT Message, 
	UINT wParam,LONG lParam) 
{ 
	return DefWindowProc(hWnd,Message,wParam,lParam); 
}

void ProcessOfCalc()
{
	STARTUPINFO cif; 
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	CreateProcess("c:\\windows\\system32\\calc.exe",NULL, 
		NULL,NULL,FALSE,NULL,NULL,NULL,&cif,&PICalc);
}

BOOL __stdcall EnumChild(HWND hWnd, LPARAM lParam)
{
	int j, k, len;
	char buf[256], new_str[3]={ 0x0d, 0x0a,0 }, temp[256], temp2[256];
	ofstream out;
	GetClassName(hWnd,temp,256);
	SendMessage(hWnd,WM_GETTEXT,sizeof(buf),(LPARAM) (LPCTSTR) buf);
	if (FFB != 0)
	{
		if (strcmp(temp,"Button")==0 && strcmp(buf,"")!=0)
		{
			out.open("names.txt",ios::app);
			if (IDOfMenuItem-SmesForButtons==20)
			{
				SmesForButtons = IDOfMenuItem;
				IDOfMenuItem++;
				strcpy(temp,"Кнопки");
				itoa(NumOfButMI,temp2,10);
				NumOfButMI++;
				strcat(temp,temp2);
				strcat(temp," 0 ");
				itoa(SmesForButtons,temp2,10);
				strcat(temp,temp2);
				out << '\n' << temp;
			}
			ArForButClick[CounterOfBut].hButton = hWnd;
			ArForButClick[CounterOfBut].IDOfMIButOfProg = IDOfMenuItem;
			itoa(SmesForButtons,temp,10);
			out << '\n' << buf << " " << temp << " ";
			itoa(IDOfMenuItem,temp,10);
			out << temp;
			out.close();
			IDOfMenuItem++;
			CounterOfBut++;
		}
	}
	else
	{
		if (strcmp(temp,"Button")==0 && strcmp(buf,"")!=0)
			CounterOfBut++;
		strcat(buf," - класс ");
		strcat(buf,temp);
		len = strlen(buf);
		for(j = 0, k = 0; j<len ; j++)
			if (buf[j]!='&')
			{
				temp[k] = buf[j];
				k++;
			}
			temp[k] = '\0';
			if (NamesOfDoughWnds[0]!='\0')
				strcat(NamesOfDoughWnds,new_str);
			strcat(NamesOfDoughWnds,temp);
	}
	return true;
}

void CountOfMIOfCalc(HMENU hOfMenuItem)
{
	int i, mOfCalcItemCount = GetMenuItemCount(hOfMenuItem);
	HMENU hOfSubMenu;
	char buf[256];
	for(i = 0; i<mOfCalcItemCount; i++)
	{
		GetMenuString(hOfMenuItem,i,buf,256,MF_BYPOSITION);
		if (strcmp(buf,"")!=0)
		{
			CounterOfMI++;
			hOfSubMenu = GetSubMenu(hOfMenuItem,i);
			if (hOfSubMenu!=0)
				CountOfMIOfCalc(hOfSubMenu);
		}
	}
}

void AddMenuOfCalcToProg(HMENU hOfMenuItem, int idOfParentMenu)
{
	int i, j, k, len, mOfCalcItemCount = GetMenuItemCount(hOfMenuItem);
	HMENU hOfSubMenu;
	ofstream out;
	char buf[256], temp[256];
	for(i = 0; i<mOfCalcItemCount; i++)
	{
		GetMenuString(hOfMenuItem,i,buf,256,MF_BYPOSITION);
		if (strcmp(buf,"")!=0)
		{
			ArForMIClick[CounterOfMI].IDOfMIOfCalc = GetMenuItemID(hOfMenuItem, i);
			ArForMIClick[CounterOfMI].IDOfMIOfProg = IDOfMenuItem;
			CounterOfMI++;
			out.open("names.txt",ios::app);
			out << '\n';
			itoa(idOfParentMenu,temp,10);
			strcat(buf," ");
			strcat(buf,temp);
			strcat(buf," ");
			itoa(IDOfMenuItem,temp,10);
			strcat(buf,temp);
			temp[0] = '\0';
			len = strlen(buf);
			for(j = 0, k = 0; j<len ; j++)
				if (buf[j]!='&')
				{
					temp[k] = buf[j];
					k++;
				}
				temp[k] = '\0';
				out << temp;
				out.close();
				hOfSubMenu = GetSubMenu(hOfMenuItem,i);
				IDOfMenuItem++;
				if (hOfSubMenu!=0)
				AddMenuOfCalcToProg(hOfSubMenu, IDOfMenuItem-1);
		}
	}
}

void ClearMenu()
{
	int i, n = GetMenuItemCount(HMenu);
	for(i = 0; i<n; i++)
		DeleteMenu(HMenu,i,MF_BYPOSITION);
}

void GetStandartMenu()
{
	ifstream in;
	ofstream out("1.txt");
	char buf[256];
	int i;
	in.open("names.txt");
	for(i = 0; i<3; i++)
	{
		in.getline(buf,sizeof(buf));
		if (i!=0)
			out << '\n';
		out << buf;
	}
	in.close();
	out.close();
	remove("names.txt");
	rename("1.txt","names.txt");
}

//функция обработки сообщений 
LRESULT CALLBACK WIN32PROC(HWND hWnd,UINT Message, 
	UINT wParam,LONG lParam) 
{
	int i;
	UINT wID;
	ofstream out;
	char buf[256], temp[256];
	switch(Message)
	{ 
	case WM_CREATE:
		i = AddMenu(hWnd);
		if (i==1)
			return 1;
		hEdit=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,(DLGPROC) WIN32DIAL);
		if (hEdit==NULL) 
			return 1;
		SizeEdit(hWnd);
		ProcessOfCalc();
		break;

	case WM_COMMAND:
		wID = LOWORD(wParam);
		if (wID<2000)
		{
			if (wID<SSmesForButtons)
			{
				//выбран пункт меню
				for(i = 0; i<CounterOfMI; i++)
				   if (ArForMIClick[i].IDOfMIOfProg==wID)
					   SendMessage(hWndOfCalc, WM_COMMAND,(WPARAM)ArForMIClick[i].IDOfMIOfCalc,0);
			}
			else
			{
				//кнопка
				for(i = 0; i<CounterOfBut; i++)
					if (ArForButClick[i].IDOfMIButOfProg==wID)
					{
						SendMessage(ArForButClick[i].hButton, WM_LBUTTONDOWN,0,0); 
						SendMessage(ArForButClick[i].hButton, WM_LBUTTONUP,0,0);
					}
			}
		}
		else
			switch(wID)
		{
			case 2001:
				NumOfButMI = 2;
				GetStandartMenu();
				NamesOfDoughWnds[0] = '\0';
				hWndOfCalc = FindWindow("SciCalc","Калькулятор");
				hMenuOfCalc = GetMenu(hWndOfCalc);
				CounterOfMI = 0;
				CountOfMIOfCalc(hMenuOfCalc);
				ArForMIClick = new ForMenuItemClick[CounterOfMI];
				IDOfMenuItem = 1;
				i = 0;
				CounterOfMI = 0;
				AddMenuOfCalcToProg(hMenuOfCalc,i);
				SSmesForButtons = SmesForButtons = IDOfMenuItem;
				IDOfMenuItem++;
				strcpy(buf,"Кнопки1 0 ");
				itoa(SmesForButtons,temp,10);
				strcat(buf,temp);
				out.open("names.txt",ios::app);
				out << '\n' << buf;
				out.close();
				CounterOfBut = 0;
				FFB = 0;
				EnumChildWindows(hWndOfCalc, EnumChild, 0);
				ArForButClick = new ForButtonClick[CounterOfBut];
				SetDlgItemText(hEdit,IDC_EDIT1,NamesOfDoughWnds);
				CounterOfBut = 0;
				FFB = 1;
				EnumChildWindows(hWndOfCalc, EnumChild, 0);
				ClearMenu();
				i = AddMenu(hWnd);
				break;
			case 2002:
				DestroyWindow(hWnd);
				break;
		}
		break;

	case WM_SIZE:
		SizeEdit(hWnd);
		break;

	case WM_DESTROY:
		TerminateProcess(PICalc.hProcess,NO_ERROR);
		GetStandartMenu();
		PostQuitMessage(0); 
		break; 

	} 
	return DefWindowProc(hWnd,Message,wParam,lParam); 
} 