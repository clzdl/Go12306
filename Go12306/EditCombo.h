#pragma once

namespace DuiLib
{
	class CComboWnd;
	class CEditWnd;
	
	
	class CEditComboUI : public CContainerUI, public IListOwnerUI
	{
		DECLARE_DUICONTROL(CEditComboUI)

		friend class CEditWnd;
		friend class CComboWnd;
	public:
		CEditComboUI();
		~CEditComboUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void DoEvent(TEventUI& event);


		int GetWindowStyls() const;

		void SetNumberOnly(bool bNumberOnly);
		bool IsPasswordMode() const;
		UINT GetMaxChar();

		TCHAR GetPasswordChar() const;
		bool IsReadOnly() const;

		DWORD GetNativeEditBkColor() const;
		DWORD GetNativeEditTextColor() const;


		//////////
		UINT GetTextStyle() const;
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetTextPadding(RECT rc);
		SIZE GetDropBoxSize() const;
		CDuiString GetDropBoxAttributeList();
		bool GetScrollSelect();
		LPCTSTR GetTipValue();
		CDuiString GetText() const;

		/////
		UINT GetListType();
		TListInfoUI* GetListInfo();
		int GetCurSel() const;
		bool SelectItem(int iIndex, bool bTakeFocus = false);
		bool SelectMultiItem(int iIndex, bool bTakeFocus = false);
		bool UnSelectItem(int iIndex, bool bOthers = false);
		bool SetItemIndex(CControlUI* pControl, int iIndex);

		void SetFont(int index);

		/////
		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		bool ActivateCoboBox();

		////
		void DoPaint(HDC hDC, const RECT& rcPaint);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		///
		void SetDisabledTextColor(DWORD dwTextColor);

		void SetShowHtml(bool bShowHtml);

		void SetNormalImage(LPCTSTR pStrImage);
		void SetHotImage(LPCTSTR pStrImage);
		void SetPushedImage(LPCTSTR pStrImage);

		void SetFocusedImage(LPCTSTR pStrImage);
		void SetDisabledImage(LPCTSTR pStrImage);
		void SetScrollSelect(bool bScrollSelect);
		void SetDropBoxAttributeList(LPCTSTR pstrList);
		void SetDropBoxSize(SIZE szDropBox);
		void SetItemTextPadding(RECT rc);
		void SetItemTextColor(DWORD dwTextColor);
		void SetItemBkColor(DWORD dwBkColor);
		void SetItemBkImage(LPCTSTR pStrImage);
		void SetAlternateBk(bool bAlternateBk);
		void SetSelectedItemTextColor(DWORD dwTextColor);
		void SetSelectedItemBkColor(DWORD dwBkColor);
		void SetSelectedItemImage(LPCTSTR pStrImage);
		void SetHotItemTextColor(DWORD dwTextColor);
		void SetHotItemBkColor(DWORD dwBkColor);
		void SetHotItemImage(LPCTSTR pStrImage);
		void SetDisabledItemTextColor(DWORD dwTextColor);
		void SetDisabledItemBkColor(DWORD dwBkColor);
		void SetDisabledItemImage(LPCTSTR pStrImage);
		void SetItemLineColor(DWORD dwLineColor);
		void SetItemShowHtml(bool bShowHtml);
		void SetReadOnly(bool bReadOnly);
		void SetPasswordMode(bool bPasswordMode);
		void SetPasswordChar(TCHAR cPasswordChar);
		void SetMaxChar(UINT uMax);
		void SetTipValue(LPCTSTR pStrTipValue);
		void SetTipValueColor(LPCTSTR pStrColor);
		void SetNativeEditBkColor(DWORD dwBkColor);
		void SetNativeEditTextColor(LPCTSTR pStrColor);


		////
		void SetPos(RECT rc, bool bNeedInvalidate);
		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);
		SIZE EstimateSize(SIZE szAvailable);


		///
		void EditTextChg();
		///
		void BoxClose();


	private:
		CEditWnd* m_pWindow;
		int m_iWindowStyls;
		bool m_bPasswordMode;
		UINT m_uMaxChar;
		TCHAR m_cPasswordChar;
		bool m_bReadOnly;
		DWORD m_dwEditbkColor;
		DWORD m_dwEditTextColor;
		UINT m_uButtonState;
		DWORD m_dwTipValueColor;


		///////// ÏÂÀ­¿ò
		CComboWnd* m_pComboWindow;

		int m_iCurSel;
		DWORD	m_dwTextColor;
		DWORD	m_dwDisabledTextColor;
		int		m_iFont;
		UINT	m_uTextStyle;
		RECT	m_rcTextPadding;
		bool	m_bShowHtml;
		CDuiString m_sDropBoxAttributes;
		SIZE m_szDropBox;

		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sPushedImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;
		CDuiString m_sTipValue;
		bool m_bScrollSelect;
		TListInfoUI m_ListInfo;
	};

}