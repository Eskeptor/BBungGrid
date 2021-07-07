#include "pch.h"
#include "BBungGrid.h"
#include <algorithm>


CBBungGrid::CBBungGrid()
{
}


CBBungGrid::~CBBungGrid()
{
}


BEGIN_MESSAGE_MAP(CBBungGrid, CMFCPropertyGridCtrl)

END_MESSAGE_MAP()


/**
Control Initialize
@access		public
@param		pFont		Font (기본값 : nullptr)
@return
*/
void CBBungGrid::Initial(CFont* pFont)
{
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);

	int nWidth = rect.Width() / GridColRatTotal;
	HDITEM hItem;
	hItem.cxy = nWidth * GridColRat_Item;
	hItem.mask = HDI_WIDTH;
	GetHeaderCtrl().SetItem(0, &HDITEM(hItem));

	EnableHeaderCtrl(TRUE, _T("Property"), _T("Value"));
	SetVSDotNetLook();
	SetCustomColors(-1, -1, RGB(202, 210, 226), -1, -1, -1, -1);
	//void SetCustomColors(COLORREF clrBackground, COLORREF clrText, COLORREF clrGroupBackground, COLORREF clrGroupText,
	//	COLORREF clrDescriptionBackground, COLORREF clrDescriptionText, COLORREF clrLine);

	if (pFont != nullptr)
		SetFont(pFont);
}


/**
Property Add
@access		public
@param		pData		Additional Property
@param		nSize		pData Size
*/
void CBBungGrid::AppendProperty(stProperty* pData, int nSize)
{
	if (pData != nullptr)
	{
		CPGridProperty* pPrevRoot = nullptr;
		CPGridProperty* pPrevSubRoot = nullptr;
		for (int i = 0; i < nSize; i++)
		{
			if (pData[i].IsGroup())
			{
				if (pData[i].GetDataType() == stProperty::VVT_ROOT)
					pPrevRoot = MakeItem(nullptr, pData[i]);
				else if (pData[i].GetDataType() == stProperty::VVT_SUBROOT)
					pPrevSubRoot = MakeItem(pPrevRoot, pData[i]);
				else if (pData[i].GetDataType() == stProperty::VVT_NONE)
				{
					if (pPrevSubRoot != nullptr)
						pPrevSubRoot = nullptr;
					else if (pPrevRoot != nullptr)
						pPrevRoot = nullptr;
				}
			}
			else
			{
				if (pPrevSubRoot != nullptr)
					MakeItem(pPrevSubRoot, pData[i]);
				else if (pPrevRoot != nullptr)
					MakeItem(pPrevRoot, pData[i]);
			}
		}
	}
}


/**
Reset Property (All Clear)
@access		public
@param
@return
*/
void CBBungGrid::ResetProperty()
{
	RemoveAll();
	m_vGridData.clear();
}


/**
Make Item (Property Item)
@access		private
@param		pParent		Parent Property
@param		stData		Data to Create
*/
CPGridProperty* CBBungGrid::MakeItem(CPGridProperty* pParent, stProperty stData)
{
	CPGridProperty* pNewProperty = nullptr;
	CString strAddItems = _T("");
	stItem propItem;

	// 프로퍼티 타입 확인
	{
		CString strItemName = stData.GetItemName();
		int nSemiCount = (int)std::count((LPCTSTR)strItemName, (LPCTSTR)strItemName + strItemName.GetLength(), _T(';'));

		CString strLine = _T("");
		CString strTemp = _T("");
		for (int i = 0; i < nSemiCount; i++)
		{
			AfxExtractSubString(strLine, strItemName, i, _T(';'));

			AfxExtractSubString(strTemp, strLine, 0, _T(':'));
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if (strTemp.Compare(VAR_TEXT) == 0)
			{
				AfxExtractSubString(strTemp, strLine, 1, _T(':'));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				stData.SetItemName(strTemp);
			}
			else if (strTemp.Compare(VAR_TYPE) == 0)
			{
				AfxExtractSubString(strTemp, strLine, 1, _T(':'));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				stData.SetPropType((stProperty::ePPropType)_ttoi(strTemp));
			}
			else if (strTemp.Compare(VAR_ARR_ITEM) == 0)
			{
				AfxExtractSubString(strTemp, strLine, 1, _T(':'));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				strAddItems = strTemp;
			}
			else if (strTemp.Compare(VAR_ENABLE) == 0)
			{
				AfxExtractSubString(strTemp, strLine, 1, _T(':'));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				strTemp.MakeUpper();
				if (strTemp.Compare(_T("TRUE")) == 0)
					stData.SetEnable(true);
				else
					stData.SetEnable(false);
			}
		}
	}

	if (stData.IsGroup())
	{
		pNewProperty = new CPGridProperty(stData.GetItemName());

		if (pParent != nullptr)
			pParent->AddSubItem(pNewProperty);
		else
			AddProperty(pNewProperty);
	}
	else
	{
		int nTemp = 0;
		void* pData = stData.GetValueP();
		if (pData == nullptr)		// 데이터가 없을 경우 임시 데이터로 초기화
			pData = &nTemp;

		if (stData.IsTest())
		{
			pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(stData.GetTestValue()), _T(""));
		}
		else
		{
			if (stData.GetPropType() == stProperty::ePPropType::PROP_GROUP)
			{
				int nSemiCount = (int)std::count((LPCTSTR)strAddItems, (LPCTSTR)strAddItems + strAddItems.GetLength(), _T(','));
				CString strTemp = _T("");
				for (int i = 0; i <= nSemiCount; i++)
				{
					AfxExtractSubString(strTemp, strAddItems, i, _T(','));

					if (i == 0)
						pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(strTemp), _T(""));

					pNewProperty->AddOption(strTemp);
				}
				pNewProperty->AllowEdit(FALSE);
			}
			else
			{
				switch (stData.GetDataType())
				{
				case stProperty::VVT_STRING:
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(CString*)pData), _T(""));
					break;
				case stProperty::VVT_INT:		// Spin이 아닌 경우에는 무조건 정수는 long int로 처리해야함
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(long int)(*(int*)pData), _T(""));
					break;
				case stProperty::VVT_LONG:
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(long*)pData), _T(""));
					break;
				case stProperty::VVT_SHORT:
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(short*)pData), _T(""));
					break;
				case stProperty::VVT_BYTE:
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(BYTE*)pData), _T(""));
					break;
				case stProperty::VVT_DWORD:
					if (stData.GetPropType() == stProperty::PROP_COLOR)
					{
						pNewProperty = new CMFCPropertyGridColorProperty(stData.GetItemName(), (_variant_t)(*(DWORD*)pData));
						((CMFCPropertyGridColorProperty*)pNewProperty)->EnableOtherButton(_T("Etc..."));
						((CMFCPropertyGridColorProperty*)pNewProperty)->EnableAutomaticButton(_T("Normal"), ::GetSysColor(COLOR_3DFACE));
					}
					else
						pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(DWORD*)pData), _T(""));
					break;
				case stProperty::VVT_DOUBLE:
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(double*)pData), _T(""));
					break;
				case stProperty::VVT_FLOAT:
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(float*)pData), _T(""));
					break;
				case stProperty::VVT_BOOL:
					pNewProperty = new CPGridProperty(stData.GetItemName(), (_variant_t)(*(bool*)pData), _T(""));
					break;
				default:
					return nullptr;
				}
			}
		}

		if (stData.GetEnable() == false)
			pNewProperty->Enable(FALSE);

		pParent->AddSubItem(pNewProperty);
	}

	if (pNewProperty != nullptr)
	{
		propItem.pProperty = pNewProperty;
		propItem.PropertyData = stData;
		m_vGridData.push_back(propItem);
	}

	return pNewProperty;
}


/**
Property Data Refresh(Linked Data)
@access		public
@param		bIsOverride		Override origin value
@return
*/
void CBBungGrid::DataRefresh(bool bIsOverride)
{
	auto iter = m_vGridData.begin();
	while (iter != m_vGridData.end())
	{
		if (iter->pProperty != nullptr)
		{
			void* pData = iter->PropertyData.GetValueP();
			if (pData != nullptr)
			{
				if (bIsOverride)
				{
					if (iter->PropertyData.GetPropType() == stProperty::ePPropType::PROP_GROUP)
					{
						if (iter->pProperty->GetOptionCount() > (*(int*)pData))
						{
							iter->pProperty->SetOriginalValue((_variant_t)iter->pProperty->GetOption((*(int*)pData)));
						}
					}
					else
					{
						switch (iter->PropertyData.GetDataType())
						{
						case stProperty::VVT_STRING:
							iter->pProperty->SetOriginalValue((_variant_t)(*(CString*)pData));
							break;
						case stProperty::VVT_INT:	// Spin이 아닌 경우에는 무조건 정수는 long int로 처리해야함
							iter->pProperty->SetOriginalValue((_variant_t)(long int)(*(int*)pData));
							break;
						case stProperty::VVT_LONG:
							iter->pProperty->SetOriginalValue((_variant_t)(*(long*)pData));
							break;
						case stProperty::VVT_SHORT:
							iter->pProperty->SetOriginalValue((_variant_t)(*(short*)pData));
							break;
						case stProperty::VVT_BYTE:
							iter->pProperty->SetOriginalValue((_variant_t)(*(BYTE*)pData));
							break;
						case stProperty::VVT_DWORD:
							iter->pProperty->SetOriginalValue((_variant_t)(*(DWORD*)pData));
							break;
						case stProperty::VVT_DOUBLE:
							iter->pProperty->SetOriginalValue((_variant_t)(*(double*)pData));
							break;
						case stProperty::VVT_FLOAT:
							iter->pProperty->SetOriginalValue((_variant_t)(*(float*)pData));
							break;
						case stProperty::VVT_BOOL:
							iter->pProperty->SetOriginalValue((_variant_t)(*(bool*)pData));
							break;
						}
					}
				}

				if (iter->PropertyData.GetPropType() == stProperty::ePPropType::PROP_GROUP)
				{
					if (iter->pProperty->GetOptionCount() > (*(int*)pData))
					{
						iter->pProperty->SetValue((_variant_t)iter->pProperty->GetOption((*(int*)pData)));
					}
				}
				else
				{
					switch (iter->PropertyData.GetDataType())
					{
					case stProperty::VVT_STRING:
						iter->pProperty->SetValue((_variant_t)(*(CString*)pData));
						break;
					case stProperty::VVT_INT:	// Spin이 아닌 경우에는 무조건 정수는 long int로 처리해야함
						iter->pProperty->SetValue((_variant_t)(long int)(*(int*)pData));
						break;
					case stProperty::VVT_LONG:
						iter->pProperty->SetValue((_variant_t)(*(long*)pData));
						break;
					case stProperty::VVT_SHORT:
						iter->pProperty->SetValue((_variant_t)(*(short*)pData));
						break;
					case stProperty::VVT_BYTE:
						iter->pProperty->SetValue((_variant_t)(*(BYTE*)pData));
						break;
					case stProperty::VVT_DWORD:
						iter->pProperty->SetValue((_variant_t)(*(DWORD*)pData));
						break;
					case stProperty::VVT_DOUBLE:
						iter->pProperty->SetValue((_variant_t)(*(double*)pData));
						break;
					case stProperty::VVT_FLOAT:
						iter->pProperty->SetValue((_variant_t)(*(float*)pData));
						break;
					case stProperty::VVT_BOOL:
						iter->pProperty->SetValue((_variant_t)(*(bool*)pData));
						break;
					}
				}
			}
		}

		iter++;
	}
}


/**
Property Data Save(Linked Data)
@access		public
@param
@return
*/
void CBBungGrid::SavePropertyData()
{
	CString strTemp = _T("");
	COleVariant oleVariant;
	auto iter = m_vGridData.begin();
	while (iter != m_vGridData.end())
	{
		if (iter->pProperty != nullptr &&
			iter->PropertyData.GetValueP() != nullptr)
		{
			oleVariant = iter->pProperty->GetValue();

			if (iter->PropertyData.GetPropType() == stProperty::ePPropType::PROP_GROUP)
			{
				strTemp = OLE2W(oleVariant.bstrVal);
				for (int i = 0; i < iter->pProperty->GetOptionCount(); i++)
				{
					if (strTemp.Compare(iter->pProperty->GetOption(i)) == 0)
					{
						*(int*)(iter->PropertyData.GetValueP()) = i;
						break;
					}
				}
			}
			else
			{
				switch (iter->PropertyData.GetDataType())
				{
				case stProperty::VVT_STRING:
				{
					USES_CONVERSION;
					*(CString*)(iter->PropertyData.GetValueP()) = OLE2W(oleVariant.Detach().bstrVal);
					break;
				}
				case stProperty::VVT_INT:	// Spin이 아닌 경우에는 무조건 정수는 long int로 처리해야함
					*(int*)(iter->PropertyData.GetValueP()) = (int)oleVariant.Detach().intVal;
					break;
				case stProperty::VVT_LONG:
					*(long*)(iter->PropertyData.GetValueP()) = (long)oleVariant.Detach().lVal;
					break;
				case stProperty::VVT_SHORT:
					*(short*)(iter->PropertyData.GetValueP()) = (short)oleVariant.Detach().iVal;
					break;
				case stProperty::VVT_BYTE:
					*(BYTE*)(iter->PropertyData.GetValueP()) = (BYTE)oleVariant.Detach().bVal;
					break;
				case stProperty::VVT_DWORD:
					*(DWORD*)(iter->PropertyData.GetValueP()) = ((CMFCPropertyGridColorProperty*)(iter->pProperty))->GetColor();
					break;
				case stProperty::VVT_DOUBLE:
					*(double*)(iter->PropertyData.GetValueP()) = (double)oleVariant.Detach().dblVal;
					break;
				case stProperty::VVT_FLOAT:
					*(float*)(iter->PropertyData.GetValueP()) = (float)oleVariant.Detach().fltVal;
					break;
				case stProperty::VVT_BOOL:
					*(bool*)(iter->PropertyData.GetValueP()) = (BOOL)oleVariant.Detach().boolVal ? true : false;
					break;
				}
			}
		}

		strTemp.Empty();
		iter++;
	}
}