// Juice Engine by Steven Ball
// Light - An OpenGL hardware light
// Created : 01/03/06
// Edited  : 01/05/06

#pragma once

#include "colour.h"
#include "../Maths/3dmaths.h"

class Light {
public:
	// Constructors
	Light() {}

	// Set/Get
	void Ambient(const Colour &ambient) { m_ambient = ambient; }
	void Diffuse(const Colour &diffuse) { m_diffuse = diffuse; }
	void Specular(const Colour &specular) { m_specular = specular; }
	void Position(const Vector3d &position) { m_position = position; }
	void Direction(const Vector3d &direction) { m_direction = direction; }
	void Exponent(const float exponent) { m_exponent = exponent; }
	void Cutoff(const float cutoff) { m_cutoff = cutoff; }
	void ConstantAttenuation(const float constantAttenuation) { m_constantAttenuation = constantAttenuation; }
	void LinearAttenuation(const float linearattenuation) { m_linearAttenuation = linearattenuation; }
	void QuadraticAttenuation(const float quadraticttenuation) { m_quadraticAttenuation = quadraticttenuation; }
	void Point(const bool point) { m_point = point; }
	void Spotlight(const bool spotlight) { m_spotlight = spotlight; }
	const Colour Ambient() const { return m_ambient; }
	const Colour Diffuse() const { return m_diffuse; }
	const Colour Specular() const { return m_specular; }
	const Vector3d Position() const { return m_position; }
	const Vector3d Direction() const { return m_direction; }
	const float Exponent() const { return m_exponent; }
	const float Cutoff() const { return m_cutoff; }
	const float ConstantAttenuation() const { return m_constantAttenuation; }
	const float LinearAttenuation() const { return m_linearAttenuation; }
	const float QuadraticAttenuation() const { return m_quadraticAttenuation; }
	const bool Point() const { return m_point; }
	const bool Spotlight() const { return m_spotlight; }

	// Apply light
	void Apply(int id) const;

	void Render();

private:
	Colour m_ambient;
	Colour m_diffuse;
	Colour m_specular;
	Vector3d m_position;
	Vector3d m_direction;
	float m_exponent;
	float m_cutoff;
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;

	bool m_point;
	bool m_spotlight;
};

inline void Light::Apply(int id) const {
	unsigned int lightNum = GL_LIGHT0 + id;

	// Set ambient color
	//GLfloat ambient[] = { m_ambient.GetRed(), m_ambient.GetGreen(), m_ambient.GetBlue() };
	glLightfv(lightNum, GL_AMBIENT, m_ambient.GetRGBA());

	// Set diffuse color
	//GLfloat diffuse[] = { m_diffuse.GetRed(), m_diffuse.GetGreen(), m_diffuse.GetBlue() };
	glLightfv(lightNum, GL_DIFFUSE, m_diffuse.GetRGBA());

	// Set specular color
	//GLfloat specular[] = { m_specular.GetRed(), m_specular.GetGreen(), m_specular.GetBlue() };
	glLightfv(lightNum, GL_SPECULAR, m_specular.GetRGBA());

	// Set light position
	GLfloat position[4];
	position[0] = m_position.x;
	position[1] = m_position.y;
	position[2] = m_position.z;

	if(m_point)
	{
		position[3] = 1.0;

		glLightfv(lightNum, GL_POSITION, position);

		if(m_spotlight)
		{
			GLfloat spot_direction[] = { m_direction.x, m_direction.y, m_direction.z };

			// Set spotlight direction
			glLightfv(lightNum, GL_SPOT_DIRECTION, spot_direction);

			// Set spotlight parameters
			glLightf(lightNum, GL_SPOT_CUTOFF, Cutoff());
		}

		glLightf(lightNum, GL_SPOT_EXPONENT, Exponent());
		glLightf(lightNum, GL_CONSTANT_ATTENUATION, ConstantAttenuation());
		glLightf(lightNum, GL_LINEAR_ATTENUATION, LinearAttenuation());
		glLightf(lightNum, GL_QUADRATIC_ATTENUATION, QuadraticAttenuation());
	}
	else {
		position[3] = 0.0;

		glLightfv(lightNum, GL_POSITION, position);
	}

	glEnable(lightNum);
}

inline void Light::Render()
{
	GLfloat lLightScale = 0.1f;

	glPushMatrix();
		glPushAttrib(GL_POLYGON_BIT);
		glPolygonMode(GL_FRONT, GL_LINE); // Wireframe mode
		glLineWidth(1.0f);

		glTranslatef(m_position.x, m_position.y, m_position.z);

		glDisable(GL_LIGHTING);

		glBegin(GL_QUADS);
			glColor3f(0.0f, 0.0f, 0.0f);

			glNormal3f(0.0f,			0.0f,			-1.0f);
			glVertex3f(lLightScale,		-lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		lLightScale,	-lLightScale);

			glNormal3f(0.0f,			0.0f,			lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	lLightScale);
			glVertex3f(lLightScale,		lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	lLightScale);

			glNormal3f(lLightScale,		0.0f,			0.0f);
			glVertex3f(lLightScale,		-lLightScale,	lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		lLightScale,	lLightScale);

			glNormal3f(-1.0f,			0.0f,			0.0f);
			glVertex3f(-lLightScale,	-lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	-lLightScale);

			glNormal3f(0.0f,			-1.0f,			0.0f);
			glVertex3f(-lLightScale,	-lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	lLightScale);

			glNormal3f(0.0f,			lLightScale,	0.0f);
			glVertex3f(lLightScale,		lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	lLightScale);
			glVertex3f(lLightScale,		lLightScale,	lLightScale);
		glEnd();

		glPopAttrib(); 

		glBegin(GL_QUADS);
			glColor3f(m_diffuse.GetRed(), m_diffuse.GetGreen(), m_diffuse.GetBlue());

			glNormal3f(0.0f,			0.0f,			-1.0f);
			glVertex3f(lLightScale,		-lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		lLightScale,	-lLightScale);

			glNormal3f(0.0f,			0.0f,			lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	lLightScale);
			glVertex3f(lLightScale,		lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	lLightScale);

			glNormal3f(lLightScale,		0.0f,			0.0f);
			glVertex3f(lLightScale,		-lLightScale,	lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		lLightScale,	lLightScale);

			glNormal3f(-1.0f,			0.0f,			0.0f);
			glVertex3f(-lLightScale,	-lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	-lLightScale);

			glNormal3f(0.0f,			-1.0f,			0.0f);
			glVertex3f(-lLightScale,	-lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	-lLightScale);
			glVertex3f(lLightScale,		-lLightScale,	lLightScale);
			glVertex3f(-lLightScale,	-lLightScale,	lLightScale);

			glNormal3f(0.0f,			lLightScale,	0.0f);
			glVertex3f(lLightScale,		lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	-lLightScale);
			glVertex3f(-lLightScale,	lLightScale,	lLightScale);
			glVertex3f(lLightScale,		lLightScale,	lLightScale);
		glEnd();

		// Render spotlight direction
		if(m_spotlight)
		{
			Vector3d lDirectionUnit = m_direction.GetUnit();

			glLineWidth(2.0f);

			glBegin(GL_LINES);
				glColor3f(m_diffuse.GetRed(), m_diffuse.GetGreen(), m_diffuse.GetBlue());

				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(lDirectionUnit.x * 1.0f, lDirectionUnit.y * 1.0f, lDirectionUnit.z * 1.0f);
			glEnd();
		}
	glPopMatrix();
}