#pragma once

class Renderer;


class Camera {
public:
	Camera(Renderer* pRenderer);

	// Set/Get
	void SetPosition(const Vector3d &position) { m_position = position; }
	void SetFacing(const Vector3d &facing) { m_facing = facing; }
	void SetUp(const Vector3d &up) { m_up = up; }
	void SetRight(const Vector3d &right) { m_right = right; }
	const Vector3d GetPosition() const { return m_position; }
	const Vector3d GetFacing()const { return m_facing; }
	const Vector3d GetUp() const { return m_up; }
	const Vector3d GetRight() const { return m_right; }

	// Camera movement
	void Fly(const float speed);
	void Move(const float speed);
	void Levitate(const float speed);
	void Strafe(const float speed);
	void Zoom(const float speed);
	void Rotate(const float xAmount, const float yAmount, const float zAmount);

	// Viewing
	void Look() const;

	static void SetLookAtCamera(const Vector3d &pos, const Vector3d &target, const Vector3d &up);

private:
	Renderer *m_pRenderer;

	Vector3d m_position;

	Vector3d m_up;
	Vector3d m_facing;
	Vector3d m_right;
};
