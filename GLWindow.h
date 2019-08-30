#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "CommonHeader.h"
#include <QtWidgets/qopenglwidget.h>

//each rotation is  +/- 120 
#define MWHEEL_SCALE_DOWN_FACTOR 120
#define STEP_DOWN 10

class SharedData;
class TestPlyModel;
class Trackball;
class TrackBallSphere;
class Frame;
class Camera;

//Qt classes
class QTimer;
class QMouseEvent;




class GLWindow: public QOpenGLWidget
{
public:
	GLWindow(SharedData* modelMVC, QWidget * parent);
	~GLWindow();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mousePressEvent(QMouseEvent* buttonPress) override;
	void mouseMoveEvent(QMouseEvent* mouseMove) override;
	void keyPressEvent(QKeyEvent* keyPress) override;
	void wheelEvent(QWheelEvent* mouseWheel) override;

private:
	//rendering objects
	//model
	TestPlyModel* plyModel;

	//trackBall
	Trackball * trackball;
	TrackBallSphere* trackBallMesh;

	//camera 
	Camera* mainCamera;
	//perspective view
	PersProjInfo_f info;

	//frame
	Frame * frameMesh;

	
private:
	void sendDataToGPU();
	void drawObjects();
	void screenToClip(Vector2f& screenCoordinate);

private:
	//screen properties
	int screenWidth, screenHeight;
	float aspectRatio;

	//mouse position
	Vector2f lastMousePos;

	//qtimer 
	QTimer* myTimer;

private slots:
	void myUpdate();

private:
	//model view controller
	SharedData* theModel;
};
#endif

