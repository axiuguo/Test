
// MFC3DView.h : CMFC3DView 类的接口
//

#pragma once


class CMFC3DView : public CView
{
protected: // 仅从序列化创建
	CMFC3DView();
	DECLARE_DYNCREATE(CMFC3DView)

// 特性
public:
	CMFC3DDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFC3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // MFC3DView.cpp 中的调试版本
inline CMFC3DDoc* CMFC3DView::GetDocument() const
   { return reinterpret_cast<CMFC3DDoc*>(m_pDocument); }
#endif

