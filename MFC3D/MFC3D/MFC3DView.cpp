
// MFC3DView.cpp : CMFC3DView ���ʵ��
//

#include "stdafx.h"
#include "glut.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFC3D.h"
#endif

#include "MFC3DDoc.h"
#include "MFC3DView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC3DView

IMPLEMENT_DYNCREATE(CMFC3DView, CView)

BEGIN_MESSAGE_MAP(CMFC3DView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC3DView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMFC3DView ����/����

CMFC3DView::CMFC3DView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFC3DView::~CMFC3DView()
{
}

BOOL CMFC3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}

// CMFC3DView ����

void CMFC3DView::OnDraw(CDC* /*pDC*/)
{
	CMFC3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	RenderScene();

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMFC3DView ��ӡ


void CMFC3DView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFC3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFC3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMFC3DView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC3DView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC3DView ���

#ifdef _DEBUG
void CMFC3DView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC3DDoc* CMFC3DView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC3DDoc)));
	return (CMFC3DDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC3DView ��Ϣ�������


int CMFC3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	z_motion = 0;
	x_motion = 0;
	y_motion = 0;
	m_rotate = 0;

	m_pDc = new CClientDC(this);
	SetTimer(1, 20, NULL);                     // �¼�IDΪ1,  20������
	InitializeOpenGL(m_pDc);                 // ��ʼ������

	Init();

	return 0;
}
BOOL CMFC3DView::InitializeOpenGL(CDC *pDc)
{
	m_pDc = pDc;
	SetupPixelFormat();                           // �������ص��ʽ
	m_hRc = ::wglCreateContext(pDc->GetSafeHdc());   // ���ɻ���������
	::wglMakeCurrent(m_pDc->GetSafeHdc(), m_hRc);  // �õ�ǰ����������
	return TRUE;
}
void CMFC3DView::Init()
{
	// ��Դֵ
	GLfloat  whiteLight[] = { 0.35f, 0.35f, 0.35f, 1.0f };
	GLfloat  sourceLight[] = { 0.65f, 0.65f, 0.65f, 1.0f };
	GLfloat    lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.60f, 0.60f, 0.60f, 1.0f);
}

void CMFC3DView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pdf = {
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С
		1,                                // �汾��
		PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ
		PFD_SUPPORT_OPENGL |              // ֧�� OpenGL
		PFD_DOUBLEBUFFER,                 // ˫����ģʽ
		PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ
		24,                               // 24 λ��ɫ���
		0, 0, 0, 0, 0, 0,                 // ������ɫλ
		0,                                // û�з�͸���Ȼ���
		0,                                // ������λλ
		0,                                // ���ۼӻ���
		0, 0, 0, 0,                       // �����ۼ�λ
		32,                               // 32 λ��Ȼ���    
		0,                                // ��ģ�建��
		0,                                // �޸�������
		PFD_MAIN_PLANE,                   // ����
		0,                                // ����
		0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ
	};
	int pixedformat;
	pixedformat = ::ChoosePixelFormat(m_pDc->GetSafeHdc(), &pdf);
	::SetPixelFormat(m_pDc->GetSafeHdc(), pixedformat, &pdf);    // �������ظ�ʽ
	if (pdf.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();                                // �����߼���ɫ��
}

void CMFC3DView::SetLogicalPalette()
{
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} logicalPalette = { 0x300, 256 };

	BYTE reds[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE greens[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE blues[] = { 0, 85, 170, 255 };

	for (int colorNum = 0; colorNum < 256; ++colorNum)
	{
		logicalPalette.aEntries[colorNum].peRed = reds[colorNum & 0x07];
		logicalPalette.aEntries[colorNum].peGreen = greens[(colorNum >> 0x03) & 0x07];
		logicalPalette.aEntries[colorNum].peBlue = blues[(colorNum >> 0x06) & 0x03];
		logicalPalette.aEntries[colorNum].peFlags = 0;
	}
	m_hPalette = CreatePalette((LOGPALETTE*)&logicalPalette);
}

void CMFC3DView::RenderScene()
{
	//     glRenderMode(GL_SELECT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ��һ�µ�ǰ��Ļ
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//����z_motion rotate��������ȷ���任�ľ�����ֵ��
	//z_motion�ı�ʱ�������ԭZ����-25��ƽ��z_motion����λ
	glTranslatef(0.0, 0.0, -30.0 + z_motion);
	//rotate�ı�ʱ��������Y�ᣨ���·�����ת5*rotate��
	glRotatef(m_rotate*5.0, 0.0, 0.0, 1.0);

	glTranslatef(0.0 + x_motion, 0.0, 0.0);
	glTranslatef(0.0, 0.0 + y_motion, 0.0);
	//  glMatrixMode(GL_SELECT);
	//  glMatrixMode(GL_PROJECTION);

	glInitNames();
	glPushName(0);
	glPushMatrix();

	glTranslatef(-6.0f, -5.0f, -20.0f);

	glLoadName(1);
	glTranslatef(15.0f, -5.0f, -20.0f);
	glLoadName(2);
	glPopName();
	glPopMatrix();
	   glTranslatef(0,0,-20);
	   int  x=(int)(40*2);
	   glBegin(GL_LINE_LOOP);
		  for(int i=-x; i<x; i+=4)
		  {
			  glVertex3i(-x,i,0);
			   glVertex3i(x,i,0);
         
			   glVertex3i(i,x,0);
			  glVertex3i(i,-x,0);
		  }
		 glEnd();

	glBegin(GL_POLYGON);
	  glVertex2f(0.0,0.0);
	  glVertex2f(0.0,3.0);
	  glVertex2f(3.0,0.0);
	  glVertex2f(3.0,3.0);

	  glEnd();

	   glPushMatrix();
			 glTranslatef(0.f,-5.0f,-40.0f);
			 glBegin(GL_TRIANGLE_STRIP);//���������Ķ������
					glColor3f(1.0,0.0,0.0);
					glVertex3f(15.0,0.0,0.0);
					glColor3f(0.0,1.0,0.0);
					glVertex3f(-15.0,0.0,0.0);
					glColor3f(0.0,0.0,1.0);
					glVertex3f(0.0,15.0,15.0);
				  //��һ������
         
					glColor3f(0.0,1.0,1.0);
					glVertex3f(10.0,15.0,-15.0);
				  //�ڶ�������
         
					glColor3f(1.0,1.0,0.0);
					glVertex3f(15.0,0.0,0.0);
			   //����������
  
				glEnd();
  
	   glPopMatrix();

	glPopMatrix();
	glFlush();
	::SwapBuffers(m_pDc->GetSafeHdc());             //����������
}


void CMFC3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//��Ӵ�������ʱ��ͼ�α任����
	glViewport(0, 0, cx, cy);                    // ��Ŀ�괰�����ã��ӿ�����
	fAspect = (GLfloat)cx / (GLfloat)cy;          // ���ڳߴ���ʣ�ΪGLfloat��
	glMatrixMode(GL_PROJECTION);         // ����ͶӰģʽ
	glLoadIdentity();                        // ���õ�ǰָ���ľ���Ϊ��λ����
	gluPerspective(45.0f, fAspect, 1.0, 425.0);    // ͸��ͼ, �ӽǣ�������zNear, zFar
	glMatrixMode(GL_MODELVIEW);         // ģ����ͼģʽ
	glLoadIdentity();
}


void CMFC3DView::OnDestroy()
{
	CView::OnDestroy();
	//ɾ����ɫ�����Ⱦ�����ġ���ʱ��
	::wglMakeCurrent(0, 0);
	::wglDeleteContext(m_hRc);       // ɾ�� ����������
	if (m_hPalette)
		DeleteObject(m_hPalette);  // ɾ�����󡰵�ɫ�塱
	if (m_pDc)
	{
		delete m_pDc;            // ɾ��DC
	}
	KillTimer(1);
}


void CMFC3DView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
}
