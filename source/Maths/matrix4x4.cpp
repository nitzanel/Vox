// ******************************************************************************
//
// Filename:	vector2d.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   4x4 matrix class implementation.
//
// Revision History:
//   Initial Revision - 11/03/06
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include <float.h>  // Required for _isnan(...)
#include <cmath>

#include "3dmaths.h"


// Constructors
Matrix4x4::Matrix4x4() {
	LoadIdentity();
}

Matrix4x4::Matrix4x4(float m[16]) {
	memcpy(this->m, m, 16 * sizeof(float));
}


// Setup matrices
void Matrix4x4::SetXRotation(const float x) {
	//LoadIdentity();

	float cos_angle = (float)cos(x);
	float sin_angle = (float)sin(x);

	if(fabs(cos_angle) < 1.0e-7f)
		cos_angle = 0;

	if(fabs(sin_angle) < 1.0e-7f)
		sin_angle = 0;

	m[5] = cos_angle;
	m[6] = sin_angle;
	m[9] = -sin_angle;
	m[10] = cos_angle;
}

void Matrix4x4::SetYRotation(const float y) {
	//LoadIdentity();

	float cos_angle = (float)cos(y);
	float sin_angle = (float)sin(y);

	if(fabs(cos_angle) < 1.0e-7f)
		cos_angle = 0;

	if(fabs(sin_angle) < 1.0e-7f)
		sin_angle = 0;

	m[0] = cos_angle;
	m[2] = -sin_angle;
	m[8] = sin_angle;
	m[10] = cos_angle;
}

void Matrix4x4::SetZRotation(const float z) {
	//LoadIdentity();

	float cos_angle = (float)cos(z);
	float sin_angle = (float)sin(z);

	if(fabs(cos_angle) < 1.0e-7f)
		cos_angle = 0;

	if(fabs(sin_angle) < 1.0e-7f)
		sin_angle = 0;
	
	m[0] = cos_angle;
	m[1] = sin_angle;
	m[4] = -sin_angle;
	m[5] = cos_angle;
}

void Matrix4x4::SetRotation(const float x, const float y, const float z) {
	Matrix4x4 rotx, roty, rotz;

	rotx.SetXRotation(x);
	roty.SetYRotation(y);
	rotz.SetZRotation(z);

	// Posible gimbal lock?
	*this = rotx * roty * rotz;
}

void Matrix4x4::SetTranslation(float trans[3]) {
	//LoadIdentity();

	m[12] = trans[0];
	m[13] = trans[1];
	m[14] = trans[2];
}

void Matrix4x4::SetTranslation(Vector3d trans) {
	//LoadIdentity();

	m[12] = trans.x;
	m[13] = trans.y;
	m[14] = trans.z;
}

void Matrix4x4::SetScale(Vector3d scale)
{
	m[0] = scale.x;
	m[5] = scale.y;
	m[10] = scale.z;
}

void Matrix4x4::AddTranslation(float *translation)
{
	m[12] = translation[0];
	m[13] = translation[1];
	m[14] = translation[2];
}

void Matrix4x4::AddRotationRadians(float *angles)
{
	double cr = cos( angles[0] );
	double sr = sin( angles[0] );
	double cp = cos( angles[1] );
	double sp = sin( angles[1] );
	double cy = cos( angles[2] );
	double sy = sin( angles[2] );

	m[0] = ( float )( cp*cy );
	m[1] = ( float )( cp*sy );
	m[2] = ( float )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	m[4] = ( float )( srsp*cy-cr*sy );
	m[5] = ( float )( srsp*sy+cr*cy );
	m[6] = ( float )( sr*cp );

	m[8] = ( float )( crsp*cy+sr*sy );
	m[9] = ( float )( crsp*sy-sr*cy );
	m[10] = ( float )( cr*cp );
}

// Properties
void Matrix4x4::GetMatrix(float* m) const {
	m[0] = this->m[0]; m[1] = this->m[1]; m[2] = this->m[2]; m[3] = this->m[3];
	m[4] = this->m[4]; m[5] = this->m[5]; m[6] = this->m[6]; m[7] = this->m[7];
	m[8] = this->m[8]; m[9] = this->m[9]; m[10] = this->m[10]; m[11] = this->m[11];
	m[12] = this->m[12]; m[13] = this->m[13]; m[14] = this->m[14]; m[15] = this->m[15];
}

const float Matrix4x4::GetDeterminant() const {
	float value;

	value = 
		m[3] * m[6] * m[9]  * m[12]-m[2] * m[7] * m[9]  * m[12]-m[3] * m[5] * m[10] * m[12]+m[1] * m[7] * m[10] * m[12]+
		m[2] * m[5] * m[11] * m[12]-m[1] * m[6] * m[11] * m[12]-m[3] * m[6] * m[8]  * m[13]+m[2] * m[7] * m[8]  * m[13]+
		m[3] * m[4] * m[10] * m[13]-m[0] * m[7] * m[10] * m[13]-m[2] * m[4] * m[11] * m[13]+m[0] * m[6] * m[11] * m[13]+
		m[3] * m[5] * m[8]  * m[14]-m[1] * m[7] * m[8]  * m[14]-m[3] * m[4] * m[9]  * m[14]+m[0] * m[7] * m[9]  * m[14]+
		m[1] * m[4] * m[11] * m[14]-m[0] * m[5] * m[11] * m[14]-m[2] * m[5] * m[8]  * m[15]+m[1] * m[6] * m[8]  * m[15]+
		m[2] * m[4] * m[9]  * m[15]-m[0] * m[6] * m[9]  * m[15]-m[1] * m[4] * m[10] * m[15]+m[0] * m[5] * m[10] * m[15];

	return value;
}

const Matrix4x4 Matrix4x4::GetNegative() const {
	Matrix4x4 negative = (*this);
	//memcpy(negative.m, this->m, 16 * sizeof(float));

	negative.Negate();

	return negative;
}

const Matrix4x4 Matrix4x4::GetTranspose() const {
	Matrix4x4 transpose = (*this);
	//memcpy(transpose.m, this->m, 16 * sizeof(float));

	transpose.Transpose();

	return transpose;
}

const Matrix4x4 Matrix4x4::GetInverse() const {
	Matrix4x4 inverse = (*this);
	//memcpy(inverse.m, this->m, 16 * sizeof(float));

	inverse.Inverse();

	return inverse;
}

const Matrix4x4 Matrix4x4::GetOrthoNormal() const {
	Matrix4x4 normal = (*this);
	//memcpy(normal.m, this->m, 16 * sizeof(float));

	normal.OrthoNormalize();

	return normal;
}

const Vector3d Matrix4x4::GetRightVector() const
{
	return Vector3d(m[0], m[1], m[2]);
}

const Vector3d Matrix4x4::GetUpVector() const
{
	return Vector3d(m[4], m[5], m[6]);
}

const Vector3d Matrix4x4::GetForwardVector() const
{
	return Vector3d(m[8], m[9], m[10]);
}

const Vector3d Matrix4x4::GetTranslationVector() const
{
	return Vector3d(m[12], m[13], m[14]);
}

const void Matrix4x4::GetEuler(float *x, float *y, float *z) const
{
	// Assuming the angles are in radians.
	if ( m[6] > 0.998f || m[6] < -0.998f )
	{
		*y = atan2( -m[8], m[0] );
		*z = 0.0f;
	}
	else
	{
		*y = atan2( m[2], m[10] );
		*z = atan2( m[4], m[5] );
	}
	*x = asin( m[6] );

	//*x = asin(m[4]);
	//*y = atan2(-m[8], m[0]);
	//*z = atan2(-m[6], m[5]);

	//if(m[0] == 1.0f)
	//{
	//	*y = atan2(m[2], m[10]);
	//	*z = 0.0f;
	//}
	//else if(m[0] == -1.0f)
	//{
	//	*y = atan2(m[2], m[10]);
	//	*z = 0.0f;
	//}
}

// Operations
void Matrix4x4::LoadIdentity() {
	memset(m, 0, 16 * sizeof(float));

	m[0] = 1;
	m[5] = 1;
	m[10] = 1;
	m[15] = 1;
}

void Matrix4x4::Negate() {
	for(int i = 0; i < 16; i++)
		m[i] = -m[i];
}

void Matrix4x4::Transpose() {
	Swap(m[1], m[4]);
	Swap(m[2], m[8]);
	Swap(m[3], m[12]);
	Swap(m[6], m[9]);
	Swap(m[7], m[13]);
	Swap(m[11],m[14]);
}

void Matrix4x4::Inverse() {
	float det = GetDeterminant();

	Matrix4x4 a;

	memcpy(a.m, this->m, 16 * sizeof(float));

	m[0]  = a.m[6]*a.m[11]*a.m[13] - a.m[7]*a.m[10]*a.m[13] + a.m[7]*a.m[9]*a.m[14] - a.m[5]*a.m[11]*a.m[14] - a.m[6]*a.m[9]*a.m[15] + a.m[5]*a.m[10]*a.m[15];
	m[1]  = a.m[3]*a.m[10]*a.m[13] - a.m[2]*a.m[11]*a.m[13] - a.m[3]*a.m[9]*a.m[14] + a.m[1]*a.m[11]*a.m[14] + a.m[2]*a.m[9]*a.m[15] - a.m[1]*a.m[10]*a.m[15];
	m[2]  = a.m[2]*a.m[7] *a.m[13] - a.m[3]*a.m[6] *a.m[13] + a.m[3]*a.m[5]*a.m[14] - a.m[1]*a.m[7] *a.m[14] - a.m[2]*a.m[5]*a.m[15] + a.m[1]*a.m[6] *a.m[15];
	m[3]  = a.m[3]*a.m[6] *a.m[9]  - a.m[2]*a.m[7] *a.m[9]  - a.m[3]*a.m[5]*a.m[10] + a.m[1]*a.m[7] *a.m[10] + a.m[2]*a.m[5]*a.m[11] - a.m[1]*a.m[6] *a.m[11];
	m[4]  = a.m[7]*a.m[10]*a.m[12] - a.m[6]*a.m[11]*a.m[12] - a.m[7]*a.m[8]*a.m[14] + a.m[4]*a.m[11]*a.m[14] + a.m[6]*a.m[8]*a.m[15] - a.m[4]*a.m[10]*a.m[15];
	m[5]  = a.m[2]*a.m[11]*a.m[12] - a.m[3]*a.m[10]*a.m[12] + a.m[3]*a.m[8]*a.m[14] - a.m[0]*a.m[11]*a.m[14] - a.m[2]*a.m[8]*a.m[15] + a.m[0]*a.m[10]*a.m[15];
	m[6]  = a.m[3]*a.m[6] *a.m[12] - a.m[2]*a.m[7] *a.m[12] - a.m[3]*a.m[4]*a.m[14] + a.m[0]*a.m[7] *a.m[14] + a.m[2]*a.m[4]*a.m[15] - a.m[0]*a.m[6] *a.m[15];
	m[7]  = a.m[2]*a.m[7] *a.m[8]  - a.m[3]*a.m[6] *a.m[8]  + a.m[3]*a.m[4]*a.m[10] - a.m[0]*a.m[7] *a.m[10] - a.m[2]*a.m[4]*a.m[11] + a.m[0]*a.m[6] *a.m[11];
	m[8]  = a.m[5]*a.m[11]*a.m[12] - a.m[7]*a.m[9] *a.m[12] + a.m[7]*a.m[8]*a.m[13] - a.m[4]*a.m[11]*a.m[13] - a.m[5]*a.m[8]*a.m[15] + a.m[4]*a.m[9] *a.m[15];
	m[9]  = a.m[3]*a.m[9] *a.m[12] - a.m[1]*a.m[11]*a.m[12] - a.m[3]*a.m[8]*a.m[13] + a.m[0]*a.m[11]*a.m[13] + a.m[1]*a.m[8]*a.m[15] - a.m[0]*a.m[9] *a.m[15];
	m[10] = a.m[1]*a.m[7] *a.m[12] - a.m[3]*a.m[5] *a.m[12] + a.m[3]*a.m[4]*a.m[13] - a.m[0]*a.m[7] *a.m[13] - a.m[1]*a.m[4]*a.m[15] + a.m[0]*a.m[5] *a.m[15];
	m[11] = a.m[3]*a.m[5] *a.m[8]  - a.m[1]*a.m[7] *a.m[8]  - a.m[3]*a.m[4]*a.m[9]  + a.m[0]*a.m[7] *a.m[9]  + a.m[1]*a.m[4]*a.m[11] - a.m[0]*a.m[5] *a.m[11];
	m[12] = a.m[6]*a.m[9] *a.m[12] - a.m[5]*a.m[10]*a.m[12] - a.m[6]*a.m[8]*a.m[13] + a.m[4]*a.m[10]*a.m[13] + a.m[5]*a.m[8]*a.m[14] - a.m[4]*a.m[9] *a.m[14];
	m[13] = a.m[1]*a.m[10]*a.m[12] - a.m[2]*a.m[9] *a.m[12] + a.m[2]*a.m[8]*a.m[13] - a.m[0]*a.m[10]*a.m[13] - a.m[1]*a.m[8]*a.m[14] + a.m[0]*a.m[9] *a.m[14];
	m[14] = a.m[2]*a.m[5] *a.m[12] - a.m[1]*a.m[6] *a.m[12] - a.m[2]*a.m[4]*a.m[13] + a.m[0]*a.m[6] *a.m[13] + a.m[1]*a.m[4]*a.m[14] - a.m[0]*a.m[5] *a.m[14];
	m[15] = a.m[1]*a.m[6] *a.m[8]  - a.m[2]*a.m[5] *a.m[8]  + a.m[2]*a.m[4]*a.m[9]  - a.m[0]*a.m[6] *a.m[9]  - a.m[1]*a.m[4]*a.m[10] + a.m[0]*a.m[5] *a.m[10];
	
	Scale( *this, 1 / det, *this);
}

void Matrix4x4::OrthoNormalize() {
	Vector3d x(m[0], m[4], m[8]);
	Vector3d y(m[1], m[5], m[9]);
	Vector3d z;

	z = Vector3d::CrossProduct(x, y); 
	y = Vector3d::CrossProduct(z, x);

	x.Normalize();
	y.Normalize();
	z.Normalize();

	m[0]  = x.x;  m[1]  = y.x;  m[2]  = z.x;  m[3]  = 0.0f;
	m[4]  = x.y;  m[5]  = y.y;  m[6]  = z.y;  m[7]  = 0.0f;
	m[8]  = x.z;  m[9]  = y.z;  m[10] = z.z;  m[11] = 0.0f;
	m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
}


void Matrix4x4::SetValues(float m[16])
{
	memcpy(this->m, m, 16 * sizeof(float));
}

void Matrix4x4::PostMultiply(Matrix4x4& matrix)
{
	float newMatrix[16];
	const float *m1 = m, *m2 = matrix.m;

	newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
	newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
	newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
	newMatrix[3] = 0;

	newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
	newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
	newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
	newMatrix[7] = 0;

	newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
	newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
	newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
	newMatrix[11] = 0;

	newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
	newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
	newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
	newMatrix[15] = 1;

	SetValues( newMatrix );
}

void Matrix4x4::InverseTranslateVector(float *pVect)
{
	pVect[0] = pVect[0] - m[12];
	pVect[1] = pVect[1] - m[13];
	pVect[2] = pVect[2] - m[14];
}

void Matrix4x4::InverseRotateVector(float *pVect)
{
	float vec[3];

	vec[0] = pVect[0]*m[0] + pVect[1]*m[1] + pVect[2]*m[2];
	vec[1] = pVect[0]*m[4] + pVect[1]*m[5] + pVect[2]*m[6];
	vec[2] = pVect[0]*m[8] + pVect[1]*m[9] + pVect[2]*m[10];

	memcpy( pVect, vec, sizeof( float )*3 );
}

void Matrix4x4::SetValues_RotALL(float x, float y, float z)
{
	Matrix4x4 rotx, roty, rotz;

	rotx.SetXRotation(x);
	roty.SetYRotation(y);
	rotz.SetZRotation(z);

	// Order is different from SetRotation() ??
	*this = rotz * roty * rotx;
}

void Matrix4x4::SetRotationRadians(float *angles)
{
	double cr = cos( angles[0] );
	double sr = sin( angles[0] );
	double cp = cos( angles[1] );
	double sp = sin( angles[1] );
	double cy = cos( angles[2] );
	double sy = sin( angles[2] );

	m[0] = ( float )( cp*cy );
	m[1] = ( float )( cp*sy );
	m[2] = ( float )( -sp );

	double srsp = sr*sp;
	double crsp = cr*sp;

	m[4] = ( float )( srsp*cy-cr*sy );
	m[5] = ( float )( srsp*sy+cr*cy );
	m[6] = ( float )( sr*cp );

	m[8] = ( float )( crsp*cy+sr*sy );
	m[9] = ( float )( crsp*sy-sr*cy );
	m[10] = ( float )( cr*cp );
}


// Arithmatic
Matrix4x4 &Matrix4x4::Add(const Matrix4x4 &m1, const Matrix4x4 &m2, Matrix4x4 &result) {
	for(int i = 0; i < 16; i++)
		result.m[i] = m1.m[i] + m2.m[i];

	return result;
}

Matrix4x4 &Matrix4x4::Subtract(const Matrix4x4 &m1, const Matrix4x4 &m2, Matrix4x4 &result) {
	for(int i = 0; i < 16; i++)
		result.m[i] = m1.m[i] - m2.m[i];

	return result;
}

Matrix4x4 &Matrix4x4::Scale(const Matrix4x4 &m1, const float &scale, Matrix4x4 &result) {
	for(int i = 0; i < 16; i++)
		result.m[i] = m1.m[i] * scale;

	result.m[0] = 1;
	result.m[5] = 1;
	result.m[10] = 1;
	result.m[15] = 1;

	return result;
}

Matrix4x4 &Matrix4x4::Multiply(const Matrix4x4 &m1, const Matrix4x4 &m2, Matrix4x4 &result) {
	double	sum;

	int	index, alpha, beta;

	for (index = 0; index < 4; index++)	{
		for (alpha = 0; alpha < 4; alpha++) {
			sum = 0.0f;

			for (beta = 0; beta < 4; beta++)
				sum += m2.m[index + beta*4] * m1.m[alpha*4 + beta];

			result.m[index + alpha*4] = (float)sum;
		}
	}

	return result;
}

Vector3d &Matrix4x4::Multiply(const Matrix4x4 &m1, const Vector3d &v, Vector3d &result) {
	int index, alpha;

	float vect[4], r[4];

	double	sum;


	vect[0] = v.x;
	vect[1] = v.y;
	vect[2] = v.z;
	vect[3] = 1;

	for(index = 0; index < 4; index++) {
		sum = 0;

		for(alpha = 0; alpha < 4; alpha++)
			sum += m1.m[index + alpha*4] * vect[alpha];

		r[index] = (float)sum;
	}

	result = Vector3d(r[0], r[1], r[2]);

	return(result);
}

bool Matrix4x4::equal(const Matrix4x4 &m1, const Matrix4x4 &m2) {
	for(int i = 0; i < 16; i++) {
		if(fabs(m1.m[i] - m2.m[i]) > 1.0e-7f)
			return false;
	}

	return true;
}


/*
// Test the Matrix4x4 class
void Matrix4x4Tester() {
	// Test all constructors
	Matrix4x4 matrix1;
	float m[16] = { 1, 2, 1, 2,
				    7, 1, 2, 4,
					6, 7, 1, 2,
					8, 3, 6, 1, };
	Matrix4x4 matrix2(m);

	cout << matrix1 << endl << endl;
	cout << matrix2 << endl << endl;


	// Test all Setup matrices
	Matrix4x4 matrix3;
	matrix3.SetXRotation(DegToRad(90.0f));
	Matrix4x4 matrix4;
	matrix4.SetYRotation(DegToRad(90.0f));
	Matrix4x4 matrix5;
	matrix5.SetZRotation(DegToRad(90.0f));
	Matrix4x4 matrix6;
	matrix6.SetRotation(DegToRad(5.0f), DegToRad(10.0f), DegToRad(15.0f));
	Matrix4x4 matrix7;
	float t[3] = { 10.0f, -4.0f, 7.5f };
	matrix7.SetTranslation(t);
	Matrix4x4 matrix8;
	Vector3d v(10.0f, -4.0f, 7.5f);
	matrix8.SetTranslation(v);

	cout << matrix3 << endl << endl;
	cout << matrix4 << endl << endl;
	cout << matrix5 << endl << endl;
	cout << matrix6 << endl << endl;
	cout << matrix7 << endl << endl;
	cout << matrix8 << endl << endl;

	// Test all properties
	cout << matrix2.GetDeterminant() << endl << endl;
	cout << matrix2.GetNegative() << endl << endl;
	cout << matrix2.GetTranspose() << endl << endl;
	cout << matrix2.GetInverse() << endl << endl;
	Matrix4x4 matrix2Inverse = matrix2.GetInverse();
	cout << matrix2Inverse * matrix2 << endl << endl;  // Should be identity!
	cout << matrix2.GetOrthoNormal() << endl << endl;
	Matrix4x4 matrix2Transpose = matrix2.GetTranspose();
	Matrix4x4 matrix2Ortho = matrix2.GetOrthoNormal();
	cout << matrix2.GetOrthoNormal().GetDeterminant() << endl << endl;  // Should be 1!

	// Test all operations
	Matrix4x4 matrix9 = matrix8;
	matrix9.LoadIdentity();
	cout << matrix9 << endl << endl;
	Matrix4x4 matrix10 = matrix2;
	matrix10.Negate();
	cout << matrix10 << endl << endl;
	Matrix4x4 matrix11 = matrix2;
	matrix11.Transpose();
	cout << matrix11 << endl << endl;
	Matrix4x4 matrix12 = matrix2;
	matrix12.Inverse();
	cout << matrix12 << endl << endl;
	Matrix4x4 matrix13 = matrix2;
	matrix13.OrthoNormalize();
	cout << matrix13 << endl << endl;

	// Test all operators
	cout << matrix2 + matrix2 << endl << endl;
	cout << matrix1 - matrix2 << endl << endl;
	cout << matrix2 * 10 << endl << endl;
	cout << matrix2 / 10 << endl << endl;

	// Test vector multiplication
	Vector3d vector1(8.0f, 0.0f, 0.0f);
	Vector3d vector2(0.0f, 7.0f, 0.0f);
	Vector3d vector3(0.0f, 0.0f, 6.0f);

	cout << matrix3 * vector1 << endl;
	cout << matrix4 * vector1 << endl;
	cout << matrix5 * vector1 << endl << endl;

	cout << matrix3 * vector2 << endl;
	cout << matrix4 * vector2 << endl;
	cout << matrix5 * vector2 << endl << endl;

	cout << matrix3 * vector3 << endl;
	cout << matrix4 * vector3 << endl;
	cout << matrix5 * vector3 << endl << endl;

	// Test streaming
	Matrix4x4 matrixIn;
	cin >> matrixIn;
	cout << matrixIn << endl;
}
*/