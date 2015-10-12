// Juice Engine by Steven Ball
// Material - An OpenGL material type
// Created : 01/05/06
// Edited  : 01/05/06

#pragma once

#include "colour.h"

class Material {
public:
	// Constructors
	Material() {}

	// Set/Get
	void Ambient(const Colour &ambient) { m_ambient = ambient; }
	void Diffuse(const Colour &diffuse) { m_diffuse = diffuse; }
	void Specular(const Colour &specular) { m_specular = specular; }
	void Emission(const Colour &emission) { m_emission = emission; }
	void Shininess(float shininess) { m_shininess = shininess; }
	const Colour Ambient() const { return m_ambient; }
	const Colour Diffuse() const { return m_diffuse; }
	const Colour Specular() const { return m_specular; }
	const Colour Emission() const { return m_emission; }
	const float Shininess() const { return m_shininess; }

	// Apply material
	void Apply() const;

private:
	Colour m_diffuse;
	Colour m_ambient;
	Colour m_specular;
	Colour m_emission;
	float m_shininess;
};

inline void Material::Apply() const {
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient.GetRGBA());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse.GetRGBA());
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular.GetRGBA());
	glMaterialfv(GL_FRONT, GL_EMISSION, m_emission.GetRGBA());
	glMaterialf(GL_FRONT, GL_SHININESS, m_shininess);
}