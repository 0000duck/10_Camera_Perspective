#include "Camera.h"



Camera::Camera()
{
	//initially the camera is at some distance
	m_cameraPosition = Vector3f(0.0f, 0.0f, 1.0f);

	//initial target origin
	m_atPosition = Vector3f(0.0f, 0.0f, 0.0f);

	//m_upPosition
	m_upPosition = Vector3f(0.0f, 1.0f, 0.0f);

	//initialise with identity matrix 
	computeCameraFrame(m_cameraPosition, m_atPosition, m_upPosition);
}



void Camera::setCameraPosition(Vector3f camPosition)
{
	//add to last position of camera
	m_cameraPosition += camPosition;
	
	//to arrest the camera flip 
	if (m_cameraPosition.z < CAMERA_Z_MIN)
		m_cameraPosition.z = CAMERA_Z_MIN;
}

void Camera::setAtPosition(Vector3f atPosition)
{
	//set the at location 
	m_atPosition = atPosition;
}

Vector3f& Camera::getCameraPosition()  
{
	return m_cameraPosition;
}

Matrix4f& Camera::getCameraConfiguration()
{
	//compute and return the value
	computeCameraFrame(m_cameraPosition, m_atPosition, m_upPosition);
	return m_cameraConfiguration;
}

void Camera::computeCameraFrame(Vector3f& camPosition, Vector3f& targetPosition, Vector3f& upPosition)
{
	//use LookAt function to generate the camera configuration(position and orientation)
	m_cameraConfiguration.LookAtRHS(camPosition, targetPosition, upPosition);
}
