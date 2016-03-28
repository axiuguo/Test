#ifndef CAPGL_H
#define CAPGL_H

#define CAP_PROP_AUTO	0

#define PLOT_TYPE_LINE	0
#define PLOT_TYPE_DOT	1
#define PLOT_TYPE_DASH	2

#define PLOT_COLOR_RED		0
#define PLOT_COLOR_BLUE		1
#define PLOT_COLOR_GREEN	2
#define PLOT_COLOR_DEFAULT	1

#include <gl/gl.h>
#include <gl/glu.h>
#include <vector>
using namespace std;

class Plot{
public:
	Plot();
	Plot(double *x,double *y,int num,int type,int color);
	Plot(double *x,double *y,int num,int type,double *color);
	~Plot();
	void SetProp(int type,int color);
	void SetProp(int type,double *color);
	void addPoint(double x,double y);
	void addPoints(double *x,double *y,int num);

	vector<double> m_points;
	int m_type;
	double m_r,m_g,m_b,m_alf;
};

class Capgl{
public:
	Capgl(CWnd *pWnd);
	~Capgl();	
	void Refresh();
	void OnSize(UINT nType, int cx, int cy);
	void vZIn(double len);
	void vZStretch(double len);
	void vXStretch(double len);
	void vYStretch(double len);
	void vXMove(double len);
	void vYMove(double len);
	void addPlot(Plot &n_plot);
	void addPlot(double *x,double *y,int num,int type,int color);
	void addPlot(double *x,double *y,int num,int type,double *color);
	void AdaptBound();
	void clearplots();
private:
	void myInitOpenGL();
	BOOL mySetupPixelFormat();
	void myDrawScene();
	HGLRC m_hRC;
	HDC m_hDC;
	int m_wid,m_hei;

	double m_vl,m_vr,m_vt,m_vb,m_vn,m_vf;

	vector<Plot> m_plots;
};

#endif