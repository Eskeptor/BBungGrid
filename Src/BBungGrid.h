/**
* 파일명			: BBungPGrid.h(BBungPGrid.cpp)
* 클래스			: CBBungPGrid
* 수퍼 클래스		: CMFCPropertyGridCtrl
* 기능			: Property Grid를 쉽게 사용할 수 있게 만들어주는(?) 클래스
* 마지막 업데이트	: 2021 / 06 / 24 - yc.jeon
* 참고			: https ://3dmpengines.tistory.com/1359
*				  https ://oceancoding.blogspot.com/2019/10/c-mfc-cpropertygridctrl.html
* 사용법
* ㄴ 원하는 폼 위에 Mfc PropertyGrid Control을 하나 만든다.
* ㄴ 해당 다이얼로그 클래스에 CBBungPGrid 변수를 하나 만든다.
* ㄴ 해당 다이얼로그 클래스의 DoDataExchange() 에서 폼위의 리소스와 CBBungPGrid 변수를 연결한다.
*	(이때 DDX_Control이 아닌 SubclassDlgItem를 사용해서 연결하면 런타임 오류가 난다.)
* ㄴ Initial 함수와 AppendProperty로 항목을 등록하고 사용한다.
* 예제
		CBBungPGrid m_ctlProperty;		// 변수 추가

		void CViewRecipeInfo1_1::DoDataExchange(CDataExchange* pDX)
		{
			CFormView::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_RECIPE_PROPERTY, m_ctlProperty);	// 변수와 컨트롤 연결
		}

		void CViewRecipeInfo1_1::InitProperty()
		{
			CBBungPGrid::stProperty GRID_STUFF[] =
			{
				P_ROOT(_T("TEXT:Cell 레시피;")),
				P_SUBROOT(_T("TEXT:Cell 데이터;")),
				P_VAR(g_RecipeManager.m_stDisplayRecipe.nRecipeNum, _T("TEXT:레시피 번호;")),
				P_VAR(g_RecipeManager.m_stDisplayRecipe.strRecipeName, _T("TEXT:레시피 명;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.nCellType, _T("TEXT:Cell 타입;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.dCellSizeX, _T("TEXT:Cell 가로 길이;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.dCellSizeY, _T("TEXT:Cell 세로 길이;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.nRepeatMode, _T("TEXT:반복 모드;")),
				P_SUBROOT_END(),
				P_SUBROOT(_T("TEXT:회전 설정;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.dRotate[0], _T("TEXT:1번 위치;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.dRotate[1], _T("TEXT:2번 위치;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.dRotate[2], _T("TEXT:3번 위치;")),
				P_VAR(g_RecipeManager.m_DisplayData.Layout.dRotate[3], _T("TEXT:4번 위치;")),
				P_SUBROOT_END(),
				P_ROOT_END(),
			};
			int nSize = sizeof(GRID_STUFF) / sizeof(CBBungPGrid::stProperty);

			m_ctlProperty.Initial();
			m_ctlProperty.AppendProperty(GRID_STUFF, nSize);
		}
*/

#pragma once

#include <vector>

#define P_VAR(P_VAL,TITLE)			{false,(&P_VAL),((CString)#P_VAL),(TITLE)}
#define P_TVAR(TITLE,TEMP_VAL)		{ (TITLE), (TEMP_VAL) }
#define P_ROOT(TITLE)				{true, CBBungGrid::stProperty::VVT_ROOT, (TITLE)}
#define P_ROOT_END()				{true, CBBungGrid::stProperty::VVT_NONE, _T("")}
#define P_SUBROOT(TITLE)			{true, CBBungGrid::stProperty::VVT_SUBROOT, (TITLE)}
#define P_SUBROOT_END()				{true, CBBungGrid::stProperty::VVT_NONE, _T("")}

constexpr LPCTSTR VAR_TEXT = _T("TEXT");
constexpr LPCTSTR VAR_TYPE = _T("TYPE");
constexpr LPCTSTR VAR_ARR_ITEM = _T("ARR");
constexpr LPCTSTR VAR_ENABLE = _T("ENABLE");

typedef CMFCPropertyGridProperty CPGridProperty;

class CBBungGrid : public CMFCPropertyGridCtrl
{
// 열거형 및 구조체 영역 ======================================================
private:
	enum eGridStuff
	{
		GridColRat_Item = 3,
		GridColRat_Value = 5,
		GridColRatTotal = GridColRat_Item + GridColRat_Value
	};

public:
	struct stProperty
	{
	public:
		enum ePDataType
		{
			VVT_ROOT = -2,
			VVT_SUBROOT = -1,
			VVT_NONE,
			VVT_STRING = 1,
			VVT_INT,
			VVT_LONG,
			VVT_SHORT,
			VVT_BYTE,
			VVT_DWORD,
			VVT_DOUBLE,
			VVT_FLOAT,
			VVT_BOOL,
		};
		enum ePPropType
		{
			PROP_NORMAL = 0,
			PROP_GROUP,
			PROP_COLOR,
			PROP_FILE,
			PROP_FOLDER,
		};

	private:
		bool bIsGroup;
		CString strItemName;
		CString strValName;
		CString strTempValue;
		bool bIsEnable;

		CString* pStrVal;
		int* pIntVal;
		long* pLongVal;
		short* pShortVal;
		BYTE* pByteVal;
		double* pDoubleVal;
		float* pFloatVal;
		bool* pBoolVal;
		DWORD* pDWVal;

		ePDataType eDataType;
		ePPropType ePropType;

	public:
		stProperty()
			: strItemName(_T("")), eDataType(VVT_NONE), strValName(_T(""))
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(false), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(CString strItemName, CString strTempValue)
			: strItemName(strItemName), eDataType(VVT_STRING), strValName(_T(""))
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(false), ePropType(PROP_NORMAL)
			, strTempValue(strTempValue), bIsEnable(true) {}

		stProperty(bool bIsGroup, ePDataType eType, CString strItemName)
			: strItemName(strItemName), eDataType(eType), strValName(_T(""))
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, CString* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_STRING), strValName(strValName)
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(pData), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, int* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_INT), strValName(strValName)
			, pIntVal(pData), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, long* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_LONG), strValName(strValName)
			, pIntVal(nullptr), pLongVal(pData), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, short* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_SHORT), strValName(strValName)
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(pData), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, BYTE* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_BYTE), strValName(strValName)
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(pData), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, DWORD* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_DWORD), strValName(strValName)
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(pData)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, double* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_DOUBLE), strValName(strValName)
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(pData), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, float* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_FLOAT), strValName(strValName)
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(pData)
			, pStrVal(nullptr), pBoolVal(nullptr)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		stProperty(bool bIsGroup, bool* pData, CString strValName, CString strItemName)
			: strItemName(strItemName), eDataType(VVT_BOOL), strValName(strValName)
			, pIntVal(nullptr), pLongVal(nullptr), pShortVal(nullptr), pByteVal(nullptr), pDWVal(nullptr)
			, pDoubleVal(nullptr), pFloatVal(nullptr)
			, pStrVal(nullptr), pBoolVal(pData)
			, bIsGroup(bIsGroup), ePropType(PROP_NORMAL), bIsEnable(true) {}

		void SetItemName(CString strName) { strItemName = strName; }
		CString GetItemName() const { return strItemName; }
		ePDataType GetDataType() const { return eDataType; }
		void SetPropType(ePPropType eType) { ePropType = eType; }
		ePPropType GetPropType() const { return ePropType; }
		bool IsGroup() const { return bIsGroup; }
		void SetEnable(bool bEnable) { bIsEnable = bEnable; }
		bool GetEnable() const { return bIsEnable; }
		void* GetValueP() const
		{
			switch (eDataType)
			{
				case ePDataType::VVT_STRING:
					return pStrVal;
				case ePDataType::VVT_INT:
					return pIntVal;
				case ePDataType::VVT_LONG:
					return pLongVal;
				case ePDataType::VVT_SHORT:
					return pShortVal;
				case ePDataType::VVT_BYTE:
					return pByteVal;
				case ePDataType::VVT_DWORD:
					return pDWVal;
				case ePDataType::VVT_DOUBLE:
					return pDoubleVal;
				case ePDataType::VVT_FLOAT:
					return pFloatVal;
				case ePDataType::VVT_BOOL:
					return pBoolVal;
				default:
					return nullptr;
			}
		}
		CString GetTestValue() const { return strTempValue; }
		bool IsTest() const { return strTempValue.IsEmpty() == false; };

		stProperty& operator=(const stProperty& ref)
		{
			bIsGroup = ref.bIsGroup;
			strItemName.Format(_T("%s"), ref.strItemName);
			strValName.Format(_T("%s"), ref.strValName);
			strTempValue.Format(_T("%s"), ref.strTempValue);
			pStrVal = ref.pStrVal;
			pIntVal = ref.pIntVal;
			pLongVal = ref.pLongVal;
			pShortVal = ref.pShortVal;
			pByteVal = ref.pByteVal;
			pDWVal = ref.pDWVal;
			pDoubleVal = ref.pDoubleVal;
			pFloatVal = ref.pFloatVal;
			pBoolVal = ref.pBoolVal;
			eDataType = ref.eDataType;
			ePropType = ref.ePropType;

			return *this;
		}
	};
	struct stItem
	{
		CPGridProperty* pProperty;
		stProperty PropertyData;

		stItem() : pProperty(nullptr) {}
	};
// ============================================================================


// 변수 영역 ==================================================================
private:
	std::vector<stItem> m_vGridData;

// ============================================================================


// 함수 영역 ==================================================================
private:
	CPGridProperty* MakeItem(CPGridProperty* pParent, stProperty stData);

protected:
	DECLARE_MESSAGE_MAP()

public:
	CBBungGrid();
	~CBBungGrid();

	// 컨트롤 초기화
	void Initial(CFont* pFont = nullptr);
	// 프로퍼티 목록 추가
	void AppendProperty(stProperty* pData, int nSize);
	// 프로퍼티 초기화 (모든 항목 제거)
	void ResetProperty();
	// 프로퍼티와 연결된 데이터 새로고침
	void DataRefresh(bool bIsOverride);
	// 프로퍼티에 연결된 데이터에 값을 저장하는 함수
	void SavePropertyData();

// ============================================================================

};

