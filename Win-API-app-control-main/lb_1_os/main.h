#ifndef _MAIN_
#define _MAIN_ 1 
#include <windows.h> 
#include "resource.h"
#include "fstream"
#include <iostream>
#include <new>

using namespace std;

struct _menu
{
	HMENU MenuItem;
	int IDOfMenuItem;
};

HINSTANCE hInst;  
static HWND hEdit;
static HMENU HMenu=NULL; 
static _menu* ArrayOfMenuItem;
static int NumberOfMenuItem;

//������� ��������� ��������� 
LRESULT CALLBACK WIN32PROC(HWND hWnd,UINT Message,UINT wParam,LONG lParam); 

//�-��� ���������� ������ �������� ��� ������ ����
int ReturnParentID(int parentID)
{
	int i;
	for(i = 0; i<NumberOfMenuItem; i++)
		if (ArrayOfMenuItem[i].IDOfMenuItem==parentID)
			return i;
	return 0;
}

//�-��� ���������� ���� � ����
int AddMenu(HWND hWnd)
{
	int i, j, k, l, m, miCounter = 0, len,
		parentID;
	int* arrayOfParentID, * tempID;
	char name[256], buf[256], temp[256];
	ifstream in;
	MENUITEMINFO* mi;
	//�������� ����  
	HMenu=CreateMenu(); 
	if (HMenu==NULL) 
		return 1;
	//������� ���-�� ������� ����
	NumberOfMenuItem = 0;
	in.open("names.txt");
	while(!in.eof())
	{
		in.getline(buf,sizeof(buf));
		NumberOfMenuItem++;
	}
	//������� ��� ������������ ��������������
	tempID = new int[NumberOfMenuItem];
	in.close();
	in.open("names.txt");
	m = 0;
	for(i = 0; i<NumberOfMenuItem; i++)
	{
		in.getline(buf,sizeof(buf));
		len = strlen(buf);
		l = 0;
		for(k = len-1; k>=0; k--)
		{
			if (buf[k]==' ')
				l++;
			if (l==2)
			{
				k++;
				break;
			}
		}
		l = 0;
		while(buf[k]!=' ')
		{
			temp[l] = buf[k];
			l++;
			k++;
		}
		temp[l] = '\0';
		parentID = atoi(temp);
		//�������� �� ���������� �����-��
		k = 0;
		for(j = 0; j<m; j++)
			if (tempID[j]==parentID)
			{
				k++;
				break;
			}
			if (k==0)
			{
				tempID[m] = parentID;
				m++;
			}
	}
	arrayOfParentID = new int[m];
	mi = new MENUITEMINFO[m];
	for(i = 0; i<m; i++)
		arrayOfParentID[i] = tempID[i];
	delete(tempID);
	ArrayOfMenuItem = new _menu[NumberOfMenuItem];
	in.close();
	in.open("names.txt");
	for(j = 0; j<NumberOfMenuItem; j++)
	{
		in.getline(buf,sizeof(buf));
		len = strlen(buf);
		ArrayOfMenuItem[j].MenuItem = CreateMenu();
		//���������� ������
		l = 0;
		for(k = len-1; k>=0; k--)
		{
			if (buf[k]==' ')
				l++;
			if (l==2)
				break;
		}
		for(l = 0; l<k; l++)
			name[l] = buf[l];
		name[l] = '\0';//�������� ������ ����
		for(k = l+1, i = 0; k<len; k++, i++)
		{
			if (buf[k]==' ')
				break;
			temp[i] = buf[k];
		}
		temp[i] = '\0';
		parentID = atoi(temp);//�������-�� ��������
		temp[0] = '\0';
		for(k = k+1, l = 0; k<len; k++, l++)
			temp[l] = buf[k];
		temp[l] = '\0';
		ArrayOfMenuItem[j].IDOfMenuItem = atoi(temp);
		//�������� �� ��������
		for(i = 0; i<m; i++)
			if (arrayOfParentID[i]==ArrayOfMenuItem[j].IDOfMenuItem)
				break;
		if (parentID==0)
			//����� ���� (��������)
			if (i!=m)
			{
				//��� ��������
				i = AppendMenu(HMenu, MF_POPUP, ArrayOfMenuItem[j].IDOfMenuItem, name);
				if (i==0) 
					return 1;
				//�������� � ���� 
				mi[miCounter].cbSize=sizeof(mi[miCounter]); 
				mi[miCounter].fMask=MIIM_SUBMENU; 
				mi[miCounter].hSubMenu=ArrayOfMenuItem[j].MenuItem; 
				i=SetMenuItemInfo(HMenu,ArrayOfMenuItem[j].IDOfMenuItem,false,&mi[miCounter]);
				if (i==0)
					return 1; 
				miCounter++;
			}
			else 
			{
				i = AppendMenu(HMenu, MF_STRING, ArrayOfMenuItem[j].IDOfMenuItem, name);
				if (i==0) 
					return 1;
			}
		else
			if (i!=m)
			{
				i = AppendMenu(ArrayOfMenuItem[ReturnParentID(parentID)].MenuItem, MF_POPUP, ArrayOfMenuItem[j].IDOfMenuItem, name);
				if (i==0) 
					return 1;
				//�������� � ���� 
				mi[miCounter].cbSize=sizeof(mi[miCounter]); 
				mi[miCounter].fMask=MIIM_SUBMENU; 
				mi[miCounter].hSubMenu=ArrayOfMenuItem[j].MenuItem; 
				i=SetMenuItemInfo(HMenu,ArrayOfMenuItem[j].IDOfMenuItem,false,&mi[miCounter]);
				if (i==0)
					return 1; 
				miCounter++;
			}
			else
			{
				i = AppendMenu(ArrayOfMenuItem[ReturnParentID(parentID)].MenuItem, MF_STRING, ArrayOfMenuItem[j].IDOfMenuItem, name);
				if (i==0) 
					return 1;
			}
	}
	in.close(); 
	//���������� ���� � ���� 
	i=SetMenu(hWnd,HMenu); 
	delete(arrayOfParentID);
	delete(mi);
	delete(ArrayOfMenuItem);
	if (i==0) 
		return 1;
	return 0;
}

int SizeEdit(HWND hWnd) 
{ 
	RECT Rect; 
	//�������� ������ ���������� ������� ���� 
	GetClientRect(hWnd,&Rect); 
	//������ ������� ���� 
	SetWindowPos(hEdit,hWnd,0,0,Rect.right-Rect.left,Rect.bottom-Rect.top,SWP_NOZORDER); 
	return 0; 
}

#endif 
