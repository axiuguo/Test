
// MFC3DView.cpp : CMFC3DView 类的实现
//

#include "stdafx.h"
#include "glut.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

// CMFC3DView 构造/析构

CMFC3DView::CMFC3DView()
{
	// TODO: 在此处添加构造代码

}

CMFC3DView::~CMFC3DView()
{
}

BOOL CMFC3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}

// CMFC3DView 绘制

void CMFC3DView::OnDraw(CDC* /*pDC*/)
{
	CMFC3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	RenderScene();

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFC3DView 打印


void CMFC3DView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFC3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFC3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CMFC3DView 诊断

#ifdef _DEBUG
void CMFC3DView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC3DDoc* CMFC3DView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC3DDoc)));
	return (CMFC3DDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC3DView 消息处理程序


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
	SetTimer(1, 20, NULL);                     // 事件ID为1,  20毫秒间距
	InitializeOpenGL(m_pDc);                 // 初始化场境

	Init();

	return 0;
}
BOOL CMFC3DView::InitializeOpenGL(CDC *pDc)
{
	m_pDc = pDc;
	SetupPixelFormat();                           // 设置像素点格式
	m_hRc = ::wglCreateContext(pDc->GetSafeHdc());   // 生成绘制描述表
	::wglMakeCurrent(m_pDc->GetSafeHdc(), m_hRc);  // 置当前绘制描述表
	return TRUE;
}
void CMFC3DView::Init()
{
	// 光源值
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
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小
		1,                                // 版本号
		PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图
		PFD_SUPPORT_OPENGL |              // 支持 OpenGL
		PFD_DOUBLEBUFFER,                 // 双缓存模式
		PFD_TYPE_RGBA,                    // RGBA 颜色模式
		24,                               // 24 位颜色深度
		0, 0, 0, 0, 0, 0,                 // 忽略颜色位
		0,                                // 没有非透明度缓存
		0,                                // 忽略移位位
		0,                                // 无累加缓存
		0, 0, 0, 0,                       // 忽略累加位
		32,                               // 32 位深度缓存    
		0,                                // 无模板缓存
		0,                                // 无辅助缓存
		PFD_MAIN_PLANE,                   // 主层
		0,                                // 保留
		0, 0, 0                           // 忽略层,可见性和损毁掩模
	};
	int pixedformat;
	pixedformat = ::ChoosePixelFormat(m_pDc->GetSafeHdc(), &pdf);
	::SetPixelFormat(m_pDc->GetSafeHdc(), pixedformat, &pdf);    // 设置像素格式
	if (pdf.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();                                // 设置逻辑调色板
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清一下当前屏幕
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//根据z_motion rotate两个参数确定变换的具体数值：
	//z_motion改变时，物体从原Z坐标-25，平移z_motion个单位
	glTranslatef(0.0, 0.0, -30.0 + z_motion);
	//rotate改变时，物体延Y轴（上下方向）旋转5*rotate度
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
			 glBegin(GL_TRIANGLE_STRIP);//画连续填充的多个三角
					glColor3f(1.0,0.0,0.0);
					glVertex3f(15.0,0.0,0.0);
					glColor3f(0.0,1.0,0.0);
					glVertex3f(-15.0,0.0,0.0);
					glColor3f(0.0,0.0,1.0);
					glVertex3f(0.0,15.0,15.0);
				  //第一个三角
         
					glColor3f(0.0,1.0,1.0);
					glVertex3f(10.0,15.0,-15.0);
				  //第二个三角
         
					glColor3f(1.0,1.0,0.0);
					glVertex3f(15.0,0.0,0.0);
			   //第三个三角
  
				glEnd();
  
	   glPopMatrix();

	glPopMatrix();
	glFlush();
	::SwapBuffers(m_pDc->GetSafeHdc());             //交互缓冲区
}


void CMFC3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//添加窗口缩放时的图形变换函数
	glViewport(0, 0, cx, cy);                    // 按目标窗口设置，视口设置
	fAspect = (GLfloat)cx / (GLfloat)cy;          // 窗口尺寸比率，为GLfloat型
	glMatrixMode(GL_PROJECTION);         // 设置投影模式
	glLoadIdentity();                        // 重置当前指定的矩阵为单位矩阵
	gluPerspective(45.0f, fAspect, 1.0, 425.0);    // 透视图, 视角，比例，zNear, zFar
	glMatrixMode(GL_MODELVIEW);         // 模型视图模式
	glLoadIdentity();
}


void CMFC3DView::OnDestroy()
{
	CView::OnDestroy();
	//删除调色板和渲染上下文、定时器
	::wglMakeCurrent(0, 0);
	::wglDeleteContext(m_hRc);       // 删除 绘制描述表
	if (m_hPalette)
		DeleteObject(m_hPalette);  // 删除对象“调色板”
	if (m_pDc)
	{
		delete m_pDc;            // 删除DC
	}
	KillTimer(1);
}


void CMFC3DView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
}
