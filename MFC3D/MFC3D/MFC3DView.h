
// MFC3DView.h : CMFC3DView ��Ľӿ�
//

#pragma once


class CMFC3DView : public CView
{
protected: // �������л�����
	CMFC3DView();
	DECLARE_DYNCREATE(CMFC3DView)

// ����
public:
	CMFC3DDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMFC3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL InitializeOpenGL(CDC *pDc);
	void Init();
	void SetupPixelFormat();
	void SetLogicalPalette();
	void RenderScene();
	HPALETTE m_hPalette;
	CDC *m_pDc;
	double z_motion;
	double x_motion;
	double y_motion;
	double m_rotate;
	float fAspect;
	HGLRC m_hRc;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // MFC3DView.cpp �еĵ��԰汾
inline CMFC3DDoc* CMFC3DView::GetDocument() const
   { return reinterpret_cast<CMFC3DDoc*>(m_pDocument); }
#endif

