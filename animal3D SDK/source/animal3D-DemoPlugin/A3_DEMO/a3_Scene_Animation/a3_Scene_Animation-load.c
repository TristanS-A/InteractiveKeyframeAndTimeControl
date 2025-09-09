/*
	Copyright 2011-2025 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein

	a3_Scene_Animation-load.c/.cpp
	Demo mode implementations: animation scene.

	********************************************
	*** LOADING FOR ANIMATION SCENE MODE     ***
	********************************************
*/

//-----------------------------------------------------------------------------
#pragma once
#include "../a3_Scene_Animation.h"

#include "../a3_DemoState.h"
#include <stdio.h>


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: ADD BLENDING
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-3: ADD IK ROUTINES
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-3
//-----------------------------------------------------------------------------

// utility to load animation
void a3animation_init_animation(a3_DemoState const* demoState, a3_Scene_Animation* scene)
{
	// general counters
	a3ui32 j, p;


	a3_FileStream fileStream[1] = { 0 };
	const a3byte* const animationStream = "./data/gpro25_base_anim_active.dat";
	const a3boolean force_disable_streaming = true;

	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD CLIPS
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------
	a3ui32 const hierarchyClipCount = 0;
	a3ui32 const hierarchySampleCount = 1;


	// stream animation assets
	const a3boolean streaming = demoState->streaming & !force_disable_streaming;
	if (streaming && a3fileStreamOpenRead(fileStream, animationStream))
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: LOAD ASSETS
//-----------------------------------------------------------------------------
		


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

		// load scene graph
		a3hierarchyLoadBinary(scene->sceneGraph, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (a3fileStreamOpenWrite(fileStream, animationStream))
	{
		// set up scenegraph
		a3hierarchyCreate(scene->sceneGraph, animationMaxCount_sceneObject, 0);
		a3hierarchySetNode(scene->sceneGraph,  0, -1, "scene_world_root");
		a3hierarchySetNode(scene->sceneGraph,  1, 0, "scene_camera_main");
		a3hierarchySetNode(scene->sceneGraph,  2, 0, "scene_light_main");
		a3hierarchySetNode(scene->sceneGraph,  3, 0, "scene_skybox");
		a3hierarchySetNode(scene->sceneGraph,  4, 0, "scene_teapot");

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD SCENE GRAPH NODES
//-----------------------------------------------------------------------------
		


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-3: REPLACE AND ADD SCENE GRAPH NODES
//-----------------------------------------------------------------------------
		


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-3
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: PREPARE ASSETS
//-----------------------------------------------------------------------------
 		


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

		// save scene graph
		a3hierarchySaveBinary(scene->sceneGraph, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}


	// map relevant objects to scene graph
	scene->obj_world_root->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_world_root");
	scene->obj_camera_main->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_camera_main");
	scene->obj_light_main->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_light_main");
	scene->obj_skybox->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skybox");
	scene->obj_teapot->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_teapot");
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD NODES
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-3: ADD REPLACE AND ADD NODES
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-3
//-----------------------------------------------------------------------------

	
	
	// scene graph state
	scene->sceneGraphState->hierarchy = 0;
	a3hierarchyStateCreate(scene->sceneGraphState, scene->sceneGraph);

	a3ui32 ammountOfsamples = 6;
	FILE* fileptr = fopen("../../../../resource/animdata/teapot.txt", "r");

	//Tristan and Will coded a preliminary reading of file indipendently to determin amount of samples per clip (one clip for now), but decided to go with Tristan's due to using fgets instead of fgetc
	char lineBuffer[256];
	while (!feof(fileptr))
	{
		fgets(lineBuffer, 2, fileptr);
		if (lineBuffer[0] == '@')
		{
			ammountOfsamples++;
		}
		else if (lineBuffer[0] == '!')
		{
			//Do other processing stuff for a new clip for multiple clips later probobly
			ammountOfsamples = 0;
		}
	}

	fseek(fileptr, 0, SEEK_SET);
	
	// initialize clip pool
	a3ui32 const hierarchyKeyframeCount = hierarchySampleCount - 1;
	a3ui32 const additionalSampleCount = ammountOfsamples;
	a3ui32 const additionalKeyframeCount = additionalSampleCount - 1;
	a3ui32 const additionalClipCount = 1;
	a3clipPoolCreate(scene->clipPool,
		hierarchyClipCount + additionalClipCount,
		hierarchyKeyframeCount + additionalKeyframeCount,
		hierarchySampleCount + additionalSampleCount);
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD CONTROLLERS
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

	// initialize additional clips and controllers
	{
		
		a3ui32 const rate_additional = 30;
		a3f64 const fps_additional = (a3f64)rate_additional;
		a3ui32 const additionalSampleStart = hierarchySampleCount;
		a3ui32 const additionalKeyframeStart = hierarchyKeyframeCount;
		a3ui32 const additionalClipStart = hierarchyClipCount;

		//Will coded the reading of the data in the file
		char name[8];
		double clipDuration;
		double startTime;
		char startChar;
		char transFrwd;
		a3_ClipTransitionFlag fwrdFlag = a3clip_stopFlag;
		char transBkwd;
		a3_ClipTransitionFlag bkwdFlag = a3clip_stopFlag;
		char hash;
		char comment[50];
		a3ui32 sampleCount = 0;
		float* ptrClipTime = NULL;
		fileptr = fopen("../../../../resource/animdata/teapot.txt", "r");

		while (fscanf(fileptr, "%c %s %lf %lf %c %c %c", &startChar, &name, &clipDuration, &startTime, &transFrwd, &transBkwd, &hash) != EOF)
		{
			printf("test %lf\t", startTime);
			fgets(comment, 50, fileptr);

			//read flag
			switch (transFrwd)
			{
			case '|':
				fwrdFlag = a3clip_stopFlag;
				break;
			case '<':
				fwrdFlag = a3clip_reverseFlag;
				break;
			case '>>':
				fwrdFlag = a3clip_playFlag;
				break;
			case '-':
				break;
			default:
				fwrdFlag = a3clip_clipFlag;
			}
			switch (transBkwd)
			{
			case '|':
				bkwdFlag = a3clip_stopFlag;
				break;
			case '<':
				bkwdFlag = a3clip_reverseFlag;
				break;
			case '>>':
				bkwdFlag = a3clip_playFlag;
				break;
			case '-':
				break;
			default:
				bkwdFlag = a3clip_clipFlag;
			}

			a3sampleInit(&scene->clipPool->sample[additionalSampleStart + sampleCount], (a3i32)startTime, fps_additional);//0.0s

			if (startChar == '!')
			{
				sampleCount = (a3ui32)0;
			}
			else
			{
				sampleCount++;
			}

		}
		fclose(fileptr);

		for (j = 0; j < additionalKeyframeCount; ++j)
			a3keyframeInit(&scene->clipPool->keyframe[additionalKeyframeStart + j],
				&scene->clipPool->sample[additionalSampleStart + j], &scene->clipPool->sample[additionalSampleStart + j + 1], fps_additional);

		j = additionalClipStart;
	/*	a3clipInit(&scene->clipPool->clip[j], "teapot_morph",
			&scene->clipPool->keyframe[additionalKeyframeStart + 0],
			&scene->clipPool->keyframe[additionalKeyframeStart + 4]);
		a3clipCalculateDuration(scene->clipPool, j, fps_additional);*/

		a3clipInitWithTransitions(&scene->clipPool->clip[j], "teapot_morph",
			&scene->clipPool->keyframe[additionalKeyframeStart + 0],
			&scene->clipPool->keyframe[additionalKeyframeStart + 4], fwrdFlag, bkwdFlag);
		a3clipCalculateDuration(scene->clipPool, j, fps_additional);


		j = a3clipGetIndexInPool(scene->clipPool, "teapot_morph");
		a3clipControllerInit(scene->clipCtrl_morph, "teapot_ctrl_morph", scene->clipPool, j, rate_additional, fps_additional);
		scene->morph_time = 0.0;
	}
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD STATES
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: BLENDING
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: SKELETON
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

	// teapot
	scene->obj_teapot->position.y = -a3real_six;
	scene->obj_teapot->scale.x = a3real_half;
	scene->obj_teapot->scaleMode = 1;


	// effectors
	// do one update to get first pose for target IK frame
	{
		void a3animation_update_animation(a3_Scene_Animation* scene, a3f64 const dt, a3boolean const updateIK);
		void a3animation_update_sceneGraph(a3_Scene_Animation* scene, a3f64 const dt);
		for (p = 0; p < 3; ++p)
		{
			a3animation_update_animation(scene, 0.0, false);
			a3animation_update_sceneGraph(scene, 0.0);

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-3: INIT IK EFFECTORS
//-----------------------------------------------------------------------------
			


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-3
//-----------------------------------------------------------------------------
		}
	}
}


//-----------------------------------------------------------------------------

void a3animation_input(a3_DemoState* demoState, a3_Scene_Animation* scene, a3f64 const dt);
void a3animation_update(a3_DemoState* demoState, a3_Scene_Animation* scene, a3f64 const dt);
void a3animation_render(a3_DemoState const* demoState, a3_Scene_Animation const* scene, a3f64 const dt);
void a3animation_input_keyCharPress(a3_DemoState const* demoState, a3_Scene_Animation* scene, a3i32 const asciiKey, a3i32 const state);
void a3animation_input_keyCharHold(a3_DemoState const* demoState, a3_Scene_Animation* scene, a3i32 const asciiKey, a3i32 const state);

void a3animation_loadValidate(a3_DemoState* demoState, a3_Scene_Animation* scene)
{
	// initialize callbacks
	a3_SceneCallbacks* const callbacks = demoState->sceneCallbacks + demoState_modeAnimation;
	callbacks->scene = scene;
	callbacks->handleInput =	(a3_Scene_EventCallback)		a3animation_input;
	callbacks->handleUpdate =	(a3_Scene_EventCallback)		a3animation_update;
	callbacks->handleRender =	(a3_Scene_EventCallbackConst)	a3animation_render;
	callbacks->handleKeyPress = (a3_Scene_InputCallback)		a3animation_input_keyCharPress;
	callbacks->handleKeyHold =	(a3_Scene_InputCallback)		a3animation_input_keyCharHold;

	// initialize cameras dependent on viewport
	scene->proj_camera_main->aspect = demoState->frameAspect;
	a3scene_updateProjectorProjectionMat(scene->proj_camera_main, true);
	a3scene_setProjectorSceneObject(scene->proj_camera_main, scene->obj_camera_main);
	// initialize cameras not dependent on viewport

	a3clipControllerRefresh(scene->clipCtrl_morph, scene->clipPool);

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: SKELETAL
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: INIT BLEND
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------
}


void a3animation_load(a3_DemoState const* demoState, a3_Scene_Animation* scene)
{
	a3ui32 i;

	a3_SceneObject* currentSceneObject;
	a3_SceneProjector* projector;


	// camera's starting orientation depends on "vertical" axis
	// we want the exact same view in either case
	const a3real sceneCameraAxisPos = 20.0f;
	const a3vec3 sceneCameraStartPos = {
		+sceneCameraAxisPos,
		-sceneCameraAxisPos,
		+sceneCameraAxisPos + 5.0f,
	};
	const a3vec3 sceneCameraStartEuler = {
		-35.0f,
		  0.0f,
		 45.0f,
	};
	const a3f32 sceneObjectDistance = 8.0f;
	const a3f32 sceneObjectHeight = 2.0f;


	// all objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
		a3scene_initSceneObject(scene->object_scene + i);
	for (i = 0; i < animationMaxCount_projector; ++i)
		a3scene_initProjector(scene->projector + i);

	currentSceneObject = scene->obj_skybox;
	currentSceneObject->scaleMode = 1;
	currentSceneObject->scale.x = 256.0f;


//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: SKELETAL
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

	// set up cameras
	projector = scene->proj_camera_main;
	projector->perspective = a3true;
	projector->fovy = a3real_fortyfive;
	projector->znear = 1.0f;
	projector->zfar = 1024.0f;
	projector->ctrlMoveSpeed = 10.0f;
	projector->ctrlRotateSpeed = 5.0f;
	projector->ctrlZoomSpeed = 5.0f;
	projector->sceneObject->position = sceneCameraStartPos;
	projector->sceneObject->euler = sceneCameraStartEuler;


	// set flags
	scene->render = animation_renderPhong;
	scene->display = animation_displayTexture;
	scene->activeCamera = animation_cameraSceneViewer;

	scene->pipeline = animation_forward;
	scene->pass = animation_passComposite;

	scene->targetIndex[animation_passScene] = animation_scene_finalcolor;
	scene->targetIndex[animation_passComposite] = animation_scene_finalcolor;

	scene->targetCount[animation_passScene] = animation_target_scene_max;
	scene->targetCount[animation_passComposite] = 1;


	// setup
	a3animation_init_animation(demoState, scene);
}


//-----------------------------------------------------------------------------
