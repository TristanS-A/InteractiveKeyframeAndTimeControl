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
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>


// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_step, playback_stepPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, A3_CLIPCTRL_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		//1. time step
		clipCtrl->clipTime_sec += dt;
		//advance the time of the keyframe
		//clipCtrl->keyframeTime_sec += dt;

		//a.pause dt = 0
		if (dt == 0)
		{
			return 0;
		}

		a3boolean isPlayingReversed = dt < 0; //IDK why we can't use a regular bool

		float clipDuration = (float)(clipCtrl->clip->duration_sec);
		//Make sure clip time is within clip bounds
		if (clipCtrl->clipTime_sec > clipDuration)
		{
			//Just clamp to end for now --> loops
			clipCtrl->clipTime_sec = clipCtrl->clipTime_sec - clipDuration;
		}
		else if (clipCtrl->clipTime_sec < 0)
		{
			//Just clamp to begining for now --> this should take us relitive to the place that we reversed to 
			//this might need to be a ABS
			clipCtrl->clipTime_sec = clipDuration + clipCtrl->clipTime_sec;
		}

		//take the current time and subtrack the duration?
		float keyFrameDuration = (float)(clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex].duration_sec);
		float keyFrameStartTime_T0 = 0;

		//This might calcualte keyframeTime_sec
		//REFACTOR THIS TO USE KEYFRAME TIME so we don't have to iterate through the whole array to find new keyframe start time
		for (a3ui32 i = 0; i < clipCtrl->keyframeIndex; i++)
		{
			keyFrameStartTime_T0 += (float)(clipCtrl->clipPool->keyframe[i].duration_sec);
		}

		float keyFrameEndTime_T1 = (float)(keyFrameStartTime_T0 + keyFrameDuration);

		//Find current keyframe (ONLY HANDLES FORWARD PLAYING LOGIC RIGHT NOW)
		while (clipCtrl->clipTime_sec >= keyFrameEndTime_T1 || clipCtrl->clipTime_sec < keyFrameStartTime_T0)
		{
			clipCtrl->keyframeIndex++;  //The locked forward playing logic

			if ((a3i32)clipCtrl->keyframeIndex < clipCtrl->clip->keyframeCount)
			{	
				keyFrameStartTime_T0 = keyFrameEndTime_T1;
				keyFrameEndTime_T1 += (float)(clipCtrl->keyframe[clipCtrl->keyframeIndex].duration_sec);
			}
			else //goes past the end of the keyframes--this is prob all wrong lol!
			{
				clipCtrl->keyframeIndex = 0;
				keyFrameEndTime_T1 = (float)(clipCtrl->clipPool->keyframe[0].duration_sec);
				keyFrameStartTime_T0 = 0;
				clipCtrl->clipTime_sec = 0;
			}
			
			//reset keyframe time becasue new keyframe
			clipCtrl->keyframeTime_sec = 0;
		}
		//Get normalized time in current keyframe with in key frame
		clipCtrl->keyframeParam = (clipCtrl->clipTime_sec - keyFrameStartTime_T0) / (keyFrameEndTime_T1 - keyFrameStartTime_T0);

		//with in clip
		clipCtrl->clipParam = (clipCtrl->clip->duration_sec - clipCtrl->clipTime_sec) / (clipCtrl->clip->duration_sec);

		//2. resolve key frame
		//		a. pause dt = 0
		//		b. forward dt > 0
		//			i.stop
		//			ii.step(s) take
		//			iii.clip exited
		//		c. reverse dt < 0
		//			i.stop
		//			ii.step(s) take
		//			iii.clip exited
		//3 normailzed keyframe/clip time: relivitve time /duration


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
