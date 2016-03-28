#include "stdafx.h"
#include "Capgl.h"

Plot::Plot()
{
}

Plot::Plot(double *x,double *y,int num,int type,int color)
{
	addPoints(x,y,num);
	SetProp(type,color);
}

Plot::Plot(double *x,double *y,int num,int type,double *color)
{
	addPoints(x,y,num);
	SetProp(type,color);
}

void Plot::SetProp(int type,int color)
{
	m_type=type;
	m_r=m_g=m_b=0;
	m_alf=1.0;
	switch(color)
	{
	case PLOT_COLOR_RED:
		m_r=1.0;
		break;
	case PLOT_COLOR_BLUE:
		m_b=1.0;
		break;
	case PLOT_COLOR_GREEN:
		m_g=1.0;
		break;
	default:
		m_b=1.0;
		break;
	}
}

void Plot::SetProp(int type,double *color)
{
	m_type=type;
	m_r=color[0];
	m_g=color[1];
	m_b=color[2];
	m_alf=color[3];
}

void Plot::addPoint(double x,double y)
{
	m_points.push_back(x);
	m_points.push_back(y);
}

void Plot::addPoints(double *x,double *y,int num)
{
	for(int i=0;i<num;i++)
	{
		m_points.push_back(x[i]);
		m_points.push_back(y[i]);
	}
}

Plot::~Plot()
{
	m_points.clear();
}

Capgl::Capgl(CWnd *pWnd)
{
	CDC *pDC=pWnd->GetDC();
	m_hDC=pDC->GetSafeHdc();
	RECT rc;
	pWnd->GetWindowRect(&rc);
	m_wid=rc.right-rc.left;
	m_hei=rc.bottom-rc.top;
	m_vl=-0.5;
	m_vr=0.5;
	m_vt=-0.5;
	m_vb=0.5;
	m_vn=1.0;
	m_vf=5.0;
	myInitOpenGL();
}

Capgl::~Capgl()
{
	wglMakeCurrent(m_hDC,NULL);//�ͷ���m_hDC ��Ӧ�� RC
	wglDeleteContext(m_hRC);//ɾ�� RC
	m_plots.clear();
}

void Capgl::Refresh()
{
	wglMakeCurrent(m_hDC,m_hRC);//ʹ RC �뵱ǰ DC �����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(m_vl,m_vr,m_vt,m_vb,m_vn,m_vf);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,m_wid,m_hei);

	myDrawScene( );//����Ļ�ͼ�������� RC �л��� 

	SwapBuffers(m_hDC);//��RC�����洫����ǰ�� DC �ϣ��Ӷ� 
					   //����Ļ����ʾ
	wglMakeCurrent(m_hDC,NULL);//�ͷ�RC���Ա����� DC ���л�ͼ 
}

void Capgl::myInitOpenGL()
{
	ASSERT(m_hDC != NULL);
	if (!mySetupPixelFormat()) //�趨��ͼ��λͼ��ʽ�����������г� 
	{
		MessageBox(NULL,L"Setup Pixel Format error!",MB_OK,NULL);
		return; 
	}
	m_hRC = wglCreateContext(m_hDC);//���� RC
	wglMakeCurrent(m_hDC, m_hRC); //RC �뵱ǰ DC �����
}

BOOL Capgl::mySetupPixelFormat()
{
	//������ʱ���ܸ�ʽ�ľ���������ʲô,�Ժ���Ϥ���ٸı��ʽ 
	static PIXELFORMATDESCRIPTOR pfd ={ 
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd 
		1, // version number 
		PFD_DRAW_TO_WINDOW | // support window 
		PFD_SUPPORT_OPENGL | // support OpenGL 
		PFD_DOUBLEBUFFER, // double buffered 
		PFD_TYPE_RGBA, // RGBA type 
		24, // 24-bit color depth 
		0, 0, 0, 0, 0, 0, // color bits ignored 
		0, // no alpha buffer 
		0, // shift bit ignored 
		0, // no accumulation buffer 
		0, 0, 0, 0, // accum bits ignored 
		32, // 32-bit z-buffer 
		0, // no stencil buffer 
		0, // no auxiliary buffer 
		PFD_MAIN_PLANE, // main layer 
		0, // reserved 
		0, 0, 0 // layer masks ignored 
	}; 
	int pixelformat; 
	if ( (pixelformat = ChoosePixelFormat(m_hDC, &pfd)) == 0 ) 
	{
		MessageBox(NULL,(L"ChoosePixelFormat failed"),MB_OK,NULL);
		return FALSE; 
	} 
	if (SetPixelFormat(m_hDC, pixelformat, &pfd) == FALSE) 
	{
		MessageBox(NULL,(L"SetPixelFormat failed"),MB_OK,NULL);
		return FALSE;
	}
	return TRUE;
}

void Capgl::OnSize(UINT nType, int cx, int cy)
{
	VERIFY(wglMakeCurrent(m_hDC,m_hRC));//ȷ��RC�뵱ǰDC���� 
	m_wid=cx; 
	m_hei=cy; 
	VERIFY(wglMakeCurrent(NULL,NULL));//ȷ��DC�ͷ�RC
}

void Capgl::myDrawScene()
{
	glClearColor(0.0f,0.0f,0.0f,1.0f);//���ñ�����ɫΪ��ɫ 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	//glPushMatrix(); 
	glLoadIdentity();
	glTranslated(0.0f,0.0f,-2.0f);//��������(0,0,-1)����ƽ�� 
	//�Ա�ͶӰʱ�ɼ�����Ϊȱʡ���ӵ���(0,0,0),ֻ���ƿ� 
	//������ܿɼ���  

	for(int i=0;i<m_plots.size();i++)
	{
		if(m_plots[i].m_type==PLOT_TYPE_LINE)glBegin(GL_LINE_STRIP); //�������߶�
		else glBegin(GL_POINTS);//������

		glColor4d(m_plots[i].m_r,m_plots[i].m_g,m_plots[i].m_b,m_plots[i].m_alf);
		for(int j=0;j<m_plots[i].m_points.size()/2;j++)
		{
			glVertex2d(m_plots[i].m_points[2*j],m_plots[i].m_points[2*j+1]);
		}
		glEnd();//���������߶�
	}
 
	glFlush(); //���� RC ��ͼ 
}

void Capgl::vZIn(double len)
{
	m_vn-=len;
	m_vf-=len;
}

void Capgl::vZStretch(double len)
{
	len*=(m_vf-m_vn);
	m_vn-=len;
	m_vf+=len;
}

void Capgl::vXMove(double len)
{
	m_vl+=len;
	m_vr+=len;
}

void Capgl::vYMove(double len)
{
	m_vt+=len;
	m_vb+=len;
}

void Capgl::vXStretch(double len)
{
	len*=(m_vr-m_vl);
	m_vl-=len;
	m_vr+=len;
}

void Capgl::vYStretch(double len)
{
	len*=(m_vb-m_vt);
	m_vt-=len;
	m_vb+=len;
}

void Capgl::addPlot(Plot &n_plot)
{
	m_plots.push_back(n_plot);
	AdaptBound();
}

void Capgl::addPlot(double *x, double *y, int num, int type, int color)
{
	Plot tmp;
	m_plots.push_back(tmp);
	m_plots[m_plots.size()-1].addPoints(x,y,num);
	m_plots[m_plots.size()-1].SetProp(type,color);
	AdaptBound();
}

void Capgl::addPlot(double *x, double *y, int num, int type, double *color)
{
	Plot tmp;
	m_plots.push_back(tmp);
	m_plots[m_plots.size()-1].addPoints(x,y,num);
	m_plots[m_plots.size()-1].SetProp(type,color);
	AdaptBound();
}

void Capgl::AdaptBound()
{
	if(m_plots.size()<1)return;
	if(m_plots[0].m_points.size()<2)return;
	m_vl=m_vr=m_plots[0].m_points[0];
	m_vt=m_vb=m_plots[0].m_points[1];
	double x,y;
	for(int i=0;i<m_plots.size();i++)
	{
		for(int j=0;j<m_plots[i].m_points.size()/2;j++)
		{
			x=m_plots[i].m_points[2*j];
			y=m_plots[i].m_points[2*j+1];
			if(x<m_vl)m_vl=x;
			if(x>m_vr)m_vr=x;
			if(y<m_vt)m_vt=y;
			if(y>m_vb)m_vb=y;
		}
	}

	if(m_vl>=m_vr)
	{
		m_vl-=0.5;
		m_vr+=0.5;
	}
	else
	{
		double len=m_vr-m_vl;
		m_vl=m_vl-0.1*len;
		m_vr=m_vr+0.1*len;
	}

	if(m_vt>=m_vb)
	{
		m_vt-=0.5;
		m_vb+=0.5;
	}
	else
	{
		double len=m_vb-m_vt;
		m_vt=m_vt-0.1*len;
		m_vb=m_vb+0.1*len;
	}
}

void Capgl::clearplots()
{
	m_plots.clear();
}