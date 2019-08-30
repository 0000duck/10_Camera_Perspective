#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include "SharedData.h"
#include "Trackball.h"
#include "TrackBallSphere.h"
#include "TestPlyModel.h"
#include "Frame.h"
#include "Camera.h"


#include "GLWindow.h"

GLWindow::GLWindow(SharedData* modelMVC, QWidget* parent): QOpenGLWidget(parent), 
screenWidth(0), screenHeight(0), theModel(modelMVC)
{
	//track ball
	trackball = new Trackball;
	trackBallMesh = new TrackBallSphere;

	//camera
	mainCamera = new Camera;

	//arrow
	frameMesh = new Frame;

	//3D ply object
	plyModel = new TestPlyModel;

	//set focus policy for capturing the keyboard
	/*must to capture the key press events*/
	setFocusPolicy(Qt::ClickFocus);
	
}


GLWindow::~GLWindow()
{
	//delete the track ball mesh after use
	if (trackball)
		delete trackball;

	if (trackBallMesh)
		delete trackBallMesh;

	//delete the ply model after use
	if (plyModel)
		delete plyModel;

	if (frameMesh)
		delete frameMesh;

	if (mainCamera)
		delete mainCamera;
}

void GLWindow::initializeGL()
{
	//initialise glew at first before using opengl functions
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "Failed to initialise the glew" << std::endl;
		exit(EXIT_FAILURE);
	}

	//opengl version and graphic card driver
	std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
	//graphic card
	std::cout << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;

	//background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	 //black color
	
	
	//load data to GPU
	sendDataToGPU();

	//set a timer
	myTimer = new QTimer;
	connect(myTimer, &QTimer::timeout, this, &GLWindow::myUpdate);
	myTimer->start(0);	//start immediately wihtout any delay

	
}

void GLWindow::sendDataToGPU()
{
	//load the trackball data
	trackBallMesh->loadData();

	//load data
	frameMesh->loadData();

	//load cube data
	plyModel->loadData();
}



void GLWindow::paintGL()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	//draw objects
	drawObjects();

	//check for any error
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error: " << err << std::endl;
	}
	
}


void GLWindow::drawObjects()
{
	Matrix4f cam, persp, finalMat;
	cam = mainCamera->getCameraConfiguration();		//get the camera position
	persp.InitPersProjTransformRHS(info);			//updated persp projection matric
	//concatenate transformation with the track ball
	finalMat = persp * cam * trackball->getTrackBallOrientation(); 


	//render the trackball 
	if (theModel->queryTrackBallFlag())
		trackBallMesh->render(finalMat);

	//draw 3D model
	plyModel->render(finalMat);

	//draw frame
	frameMesh->render(finalMat);
}

void GLWindow::screenToClip(Vector2f& screenCoordinate)
{
	//y coordinate
	screenCoordinate.y = 1.0f - (screenCoordinate.y/ (0.5f * screenHeight) );
	screenCoordinate.x = (screenCoordinate.x / (0.5f * screenWidth)) - 1.0f;
}

void GLWindow::myUpdate()
{
	//draw again
	static unsigned int counter = 0;
	counter++;
	if (counter % 100 == 0)
	{
		repaint();
		counter = 0;
	}
}



void GLWindow::resizeGL(int w, int h)
{
	//update the screen size
	screenWidth = w;
	screenHeight = h;

	//update the information about the perspective mprojection
	info.FOV = 90.0f;	//Field of view
	info.Width = screenWidth;
	info.Height = screenHeight;
	info.zNear = 0.1f;			//near plane
	info.zFar = 100.0f;				//far plane
}

void GLWindow::mousePressEvent(QMouseEvent* buttonPress)
{
	if (buttonPress->buttons() & Qt::LeftButton)
	{
		//current mouse position
		QPoint mousePos = buttonPress->pos();

		//covert the mouse movement points from pixel->Normalized Coordinates
		lastMousePos = Vector2f(mousePos.x(), mousePos.y());
		screenToClip(lastMousePos);

		//set the position of trackball
		if (theModel->queryTrackBallFlag())
			trackball->setMousePosition(lastMousePos);
	}
	

	//ask opengl to render
	repaint();
}

void GLWindow::mouseMoveEvent(QMouseEvent* Move)
{
	//check whether the left button is pressed
	if (Move->buttons() == Qt::LeftButton)
	{
		//current mouse position
		QPoint mousePos = Move->pos();

		//convert the mouse movement points from pixel->Normalized Coordinates
		Vector2f currentMousePos(mousePos.x(), mousePos.y());
		screenToClip(currentMousePos);

		//if trackball is reuired 
		if (theModel->queryTrackBallFlag())
		{
			//send the two vectors to Trackball
			trackball->computeTrackballAxisAngle(currentMousePos);
		}

		//update mouse position
		lastMousePos = currentMousePos;
		
	}

	
	
}

void GLWindow::keyPressEvent(QKeyEvent* keyPress)
{
	//If UP key is pressed do something
	if(keyPress->key() == Qt::Key_Up)
		std::cout << "Up Key is pressed" << std::endl;

	//If Down key is pressed do something
	if (keyPress->key() == Qt::Key_Down)
		std::cout << "Down Key is pressed" << std::endl;
}

void GLWindow::wheelEvent(QWheelEvent* mouseWheel)
{
	//calculate the increment from mouse rotation
	float increment = (float)mouseWheel->delta() / (MWHEEL_SCALE_DOWN_FACTOR * STEP_DOWN);
	//set the new camera position
	mainCamera->setCameraPosition(Vector3f(0.0f, 0.0f, increment));
}


