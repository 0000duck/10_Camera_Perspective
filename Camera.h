#ifndef CAMERA_H
#define CAMERA_H
#include "CommonHeader.h"

#define CAMERA_Z_MIN 0.001
class Camera
{
public:
	Camera();


	//setters 
	//camera Position
	void setCameraPosition(Vector3f camPosition);	
	void setAtPosition(Vector3f atPosition);

	//getters
	//camera position
	Vector3f& getCameraPosition();

	//camera configuration
	Matrix4f& getCameraConfiguration();

	

private:
	Vector3f m_upPosition; // camera up direction
	Vector3f m_atPosition;	//where to look
	Vector3f m_cameraPosition;	//location of camera
	Matrix4f m_cameraConfiguration;	//position and orientaton of camera frame
	
	//calculate the cameraFrame
	void computeCameraFrame(Vector3f& camPosition, Vector3f& targetPosition, Vector3f& upPosition);

};
#endif // !CAMERA_H



