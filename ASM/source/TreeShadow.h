#ifndef TREE_SHADOW_INCLUDED
#define TREE_SHADOW_INCLUDED

#include "SM64DS_2.h"

struct TreeShadow : public Actor
{
	Model model;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	unsigned opacity;
	
	void UpdateModelTransform();
	
	static TreeShadow* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~TreeShadow() override;
	
	static SharedFilePtr modelFile;
	
	static SpawnInfo<TreeShadow> spawnData;
};

#endif