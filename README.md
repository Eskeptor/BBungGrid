# Welcome to my "BBungGrid""
## About this project
Class that makes "CMFCPropertyGridCtrl" easier to use.
![MAIN1](/ScreenShot/1.PNG)
## What's New? (Last updated 2021.07.07)
```
 1) Add Project
```
## Next Updates
```
 1) Add Other Type Property
 2) Bug Fix (Changed Item)
```
## How To Use
### 1) Add Control (CMFCPropertyGridCtrl) and Link
![Image](/ScreenShot/2.PNG)
```cpp
void CYourDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, MFCPROPERTYGRID_ID, m_ctlPropGrid);	// <--
}
```
### 2) Init Property
```cpp
CBBungGrid::stProperty GRID_STUFF[] =
{
	P_ROOT(_T("TEXT:Cell Data;")),					// <-- Root
		P_SUBROOT(_T("TEXT:Cell Data;")),			// <-- Sub Root
			P_VAR(m_strDataName, _T("TEXT:Data Name;ENABLE:FALSE;")),
			P_VAR(m_nX, _T("TEXT:X;")),
			P_VAR(m_nY, _T("TEXT:Y;")),
			P_VAR(m_dTheta, _T("TEXT:Theta;")),
			P_VAR(m_nType, _T("TEXT:Type;TYPE:1;ARR:Type1,Type2,Type3;")),
		P_SUBROOT_END(),							// <-- Sub Root End
		P_SUBROOT(_T("TEXT:Other;")),				// <-- Sub Root
			P_VAR(m_clrColor, _T("TEXT:Color;TYPE:2;")),
		P_SUBROOT_END(),							// <-- Sub Root End
	P_ROOT_END(),									// <-- Root End
};
int nSize = sizeof(GRID_STUFF) / sizeof(CBBungGrid::stProperty);

m_ctlPropGrid.Initial(&m_font);
m_ctlPropGrid.AppendProperty(GRID_STUFF, nSize);
```
### 3) Property Type
- P_ROOT
  - Root Item
  - Args : Text
- P_ROOT_END
  - Root End Item
  - "P_ROOT" and "P_ROOT_END" must be paired.
- P_SUBROOT
  - Sub Root Item
  - Args : Text
- P_SUBROOT_END
  - Sub Root End Item
  - "P_SUBROOT" and "P_SUBROOT_END" must be paired.
- P_VAR
  - Property Item
  - Args : Linked Variable, Text
  - Normal Property Type : PROP_NORMAL
  - Ex
    - P_VAR(m_nX, _T("TEXT:X;"))
	  - Linked Variable = m_nX
	  - Text = X
	- P_VAR(m_nType, _T("TEXT:Type;TYPE:1;ARR:Type1,Type2,Type3;"))
	  - Linked Variable = m_nType
	  - Text = Type
	  - Property Type = 1 (ComboBox Type)
	  - ComboBox Item = Type1, Type2, Type3
	- P_VAR(m_clrColor, _T("TEXT:Color;TYPE:2;"))
	  - Linked Variable = m_clrColor
	  - Text = Color
	  - Property Type = 2 (Color Selector)
- P_TVAR
  - Property Item (No Linked Variable)
  - Args : Text
  - Ex
    - P_TVAR(_T("TEXT:Just String"))
## License
```
 1) CMFCPropertyGridProperty - Microsoft Corporation
```
## Good Bye!!
Compiled by Visual Studio 2019