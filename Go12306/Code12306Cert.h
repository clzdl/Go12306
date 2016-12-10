#pragma once




class CCode12306CertUI : public CControlUI
{
	DECLARE_DUICONTROL(CCode12306CertUI)
public:

	CCode12306CertUI();
	~CCode12306CertUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	
	void DoEvent(TEventUI& event);
	void DoInit();
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetNormalImage(CDuiString v) { m_sNormalImage = v; }

	std::vector<CDuiPoint> GetSelectedPoint();

private:

	CDuiString m_sNormalImage;

	std::string m_ortImageBytes;

	std::vector<CDuiRect> m_vecClickPoints;

};
