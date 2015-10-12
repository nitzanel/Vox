#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
	mMinX = 0.0f;
	mMinY = 0.0f;
	mMinZ = 0.0f;

	mMaxX = 0.0f;
	mMaxY = 0.0f;
	mMaxZ = 0.0f;
}

BoundingBox::~BoundingBox()
{

}

float BoundingBox::GetWidth() const
{
	return mMaxX - mMinX;
}

float BoundingBox::GetLength() const
{
	return mMaxZ - mMinZ;
}

float BoundingBox::GetHeight() const
{
	return mMaxY - mMinY;
}

Vector3d BoundingBox::GetCenter() const
{
	return Vector3d(mMaxX - mMinX * 0.5f, mMaxY - mMinY * 0.5f, mMaxY - mMinY * 0.5f);
}

float BoundingBox::GetMinX() const
{
	return mMinX;
}

float BoundingBox::GetMinY() const
{
	return mMinY;
}

float BoundingBox::GetMinZ() const
{
	return mMinZ;
}

float BoundingBox::GetMaxX() const
{
	return mMaxX;
}

float BoundingBox::GetMaxY() const
{
	return mMaxY;
}

float BoundingBox::GetMaxZ() const
{
	return mMaxZ;
}