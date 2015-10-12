#pragma once

#include "../Renderer/Renderer.h"
#include "MS3DModel.h"

// Joint animation structure
typedef struct JointAnimation
{
	int currentTranslationKeyframe;
	int currentRotationKeyframe;

	float startBlendTrans[3];
	float endBlendTrans[3];
	float startBlendRot[3];
	float endBlendRot[3];
	float currentBlendTrans[3];
	float currentBlendRot[3];

	Matrix4x4 final;

} JointAnimation;

// Animation structure
static const int MAX_ANIMATION_NAME = 64;
typedef struct Animation
{
	int startFrame;
	int endFrame;
	int blendFrame;
	double startTime;
	double endTime;
	bool looping;
	char animationName[MAX_ANIMATION_NAME];
} Animation;


class MS3DAnimator
{
public:
	MS3DAnimator(Renderer *lpRenderer, MS3DModel* pModel);
	~MS3DAnimator();

	MS3DModel* GetModel();

	void CreateJointAnimations();

	bool LoadAnimations(const char *animationFileName);

	void CalculateBoundingBox();
	BoundingBox* GetBoundingBox();

	void PlayAnimation(int lAnimationIndex);
	void PlayAnimation(const char *lAnimationName);
	void PauseAnimation();
	void ResumeAnimation();

	int GetCurrentAnimationIndex() const;
	int GetNumAnimations() const;
	const char *GetAnimationName(const int index);
	bool IsAnimationPaused() const;
	bool HasAnimationLooped() const;
	bool HasAnimationFinished() const;

	int GetStartFrame(const char *lAnimationName);
	int GetEndFrame(const char *lAnimationName);
	int GetCurrentFrame();

	// Blending
	void StartBlendAnimation(int startIndex, int endIndex, float blendTime);
	void StartBlendAnimation(const char *lStartAnimationName, const char *lEndAnimationName, float blendTime);
	void BlendIntoAnimation(const char *lAnimationName, float blendTime);

	void GetCurrentBlendTranslation(int jointIndex, float* x, float* y, float* z);
	void GetCurrentBlendRotation(int jointIndex, float* x, float* y, float* z);

	void Restart();

	void SetTimerForStartOfAnimation();

	Matrix4x4 GetBoneMatrix(int index);

	void Update(float dt);
	void UpdateBlending(float dt);

	void Render(bool lMesh, bool lNormals, bool lBones, bool lBoundingBox);
	void RenderMesh();
	void RenderNormals();
	void RenderBones();
	void RenderBoundingBox();

private:
	Renderer *mpRenderer;

	MS3DModel* mpModel;

	// Joint animations
	int numJointAnimations;
	JointAnimation *pJointAnimations;

	// Animations
	int numAnimations;
	Animation *pAnimations;

	// Current playing animation
	int mCurrentAnimationIndex;
	double mCurrentAnimationStartTime;
	double mCurrentAnimationEndTime;

	// Current animation timer and local looping variable
	double m_timer;
	bool mblooping;

	bool m_bPaused;
	bool m_bLooped;
	bool m_bFinished;

	// Blending
	bool m_bBlending;
	float m_blendTime;
	double m_blendTimer;
	int m_blendStartAnimationIndex;
	int m_blendEndAnimationIndex;

	// Bounding box
	BoundingBox m_BoundingBox;
};
