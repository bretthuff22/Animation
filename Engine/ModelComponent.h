#ifndef INCLUDED_ENGINE_MODELCOMPONENT_H
#define INCLUDED_ENGINE_MODELCOMPONENT_H

//====================================================================================================
// Filename:	ModelComponent.h
// Created by:	Peter Chan
// Description:	Class for the model component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Model.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class ModelComponent
{
public:
	ModelComponent();
	~ModelComponent();

	void SetModel(const Model* model)	{ mModel = model; }
	const Model* GetModel() const		{ return mModel; }

private:
	const Model* mModel;
};

#endif // #ifndef INCLUDED_ENGINE_MODELCOMPONENT_H