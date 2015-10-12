// Juice Engine by Steven Ball
// Colour - A simple 4 float colour class
// Created : 26/03/06
// Edited  : 26/03/06

#pragma once

#include <cmath>
#include <iostream>
using namespace std;

class Colour {
public:
	// Constructors
	Colour() { m_rgba[0] = m_rgba[1] = m_rgba[2] = m_rgba[3] = 0.0f; }
	Colour(float r, float g, float b, float a = 1.0f) {
		m_rgba[0] = r;
		m_rgba[1] = g;
		m_rgba[2] = b;
		m_rgba[3] = a;
	}
	Colour(float rgba[]) {
		m_rgba[0] = rgba[0];
		m_rgba[1] = rgba[1];
		m_rgba[2] = rgba[2];
		m_rgba[3] = rgba[3];
	}

	// Set/Get
	void SetRed(const float red) { m_rgba[0] = red; }
	void SetGreen(const float green) { m_rgba[1] = green; }
	void SetBlue(const float blue) { m_rgba[2] = blue; }
	void SetAlpha(const float alpha) { m_rgba[3] = alpha; }
	const float GetRed() const { return m_rgba[0]; }
	const float GetGreen() const { return m_rgba[1]; }
	const float GetBlue() const { return m_rgba[2]; }
	const float GetAlpha() const { return m_rgba[3]; }
	const float* GetRGBA() const { return m_rgba; }

    static void RGBToHSV(float r, float g, float b, float *h, float *s, float *v);
    static void HSVToRGB(float h, float s, float v, float *r, float *g, float *b);

private:
	float m_rgba[4];
};