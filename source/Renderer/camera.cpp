#include <cmath>

#include "../Maths/3dmaths.h"
#include "Renderer.h"

#include "camera.h"


// Angle conversions
inline float DegToRad(const float degrees)
{
	return (degrees * PI) / 180;
}

inline float RadToDeg(const float radians)
{
	return (radians * 180) / PI;
}


// Constructor
Camera::Camera(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	SetPosition(Vector3d(0.0f, 0.0f, 0.0f));
	SetFacing(Vector3d(0.0f, 0.0f, -1.0f));
	SetUp(Vector3d(0.0f, 1.0f, 0.0f));
	SetRight(Vector3d(1.0f, 0.0f, 0.0f));
}


// Camera movement
void Camera::Fly(const float speed)
{
	m_position.x = m_position.x + m_facing.x * speed;
	m_position.y = m_position.y + m_facing.y * speed;
	m_position.z = m_position.z + m_facing.z * speed;
}

void Camera::Move(const float speed)
{
	Vector3d directionToMove = m_facing;
	directionToMove.y = 0.0f;
	directionToMove.Normalize();

	m_position.x = m_position.x + directionToMove.x * speed;
	m_position.z = m_position.z + directionToMove.z * speed;
}

void Camera::Levitate(const float speed)
{
	m_position.y = m_position.y + 1.0f * speed;
}

void Camera::Strafe(const float speed)
{
	m_position.x = m_position.x + m_right.x * speed;
	m_position.y = m_position.y + m_right.y * speed;
	m_position.z = m_position.z + m_right.z * speed;
}

void Camera::Zoom(const float speed)
{
}

void Camera::Rotate(const float xAmount, const float yAmount, const float zAmount)
{
	Quaternion xRotation;
	xRotation.SetAxis(m_right, xAmount);
	Quaternion yRotation;
	yRotation.SetAxis(m_up, yAmount);
	Quaternion zRotation;
	zRotation.SetAxis(m_facing, zAmount);

	Quaternion rotation = xRotation * yRotation * zRotation;

	m_right = (rotation * m_right).GetUnit();
	m_up = (rotation * m_up).GetUnit();
	m_facing = (rotation * m_facing).GetUnit();
}

// Viewing
void Camera::Look() const {
	Vector3d view = m_position + m_facing;
	gluLookAt(m_position.x, m_position.y, m_position.z, view.x, view.y, view.z, m_up.x, m_up.y, m_up.z);
    m_pRenderer->GetFrustum(m_pRenderer->GetActiveViewPort())->SetCamera(m_position, view, m_up);
}

void Camera::SetLookAtCamera(const Vector3d &pos, const Vector3d &target, const Vector3d &up) {
	gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
}