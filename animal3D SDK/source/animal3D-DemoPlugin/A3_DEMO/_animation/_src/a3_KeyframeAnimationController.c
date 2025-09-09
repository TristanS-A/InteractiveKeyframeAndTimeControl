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

#include <assert.h>


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
		
		////Handles pause - Tristan
		if (dt == 0 || clipCtrl->playback_sec == 0 || clipCtrl->clip->keyframeDirection == 0)
		{
			return 0;
		}

		////Updates clip time --> nice touch for scaling with time! - Tristan created and Will refactored/imroved
		clipCtrl->clipTime_sec += dt * clipCtrl->playback_sec * clipCtrl->clipPool->clip->keyframeDirection;

		//Will
		a3f64 clipDuration = (clipCtrl->clip->duration_sec);

		////Handles transitions - Tristan coded switches and Stop and Loop transitions. Will coded Ping Pong.
		//For forward transition if clip time exceeds clip duration
		if (clipCtrl->clipTime_sec > clipDuration)
		{
			a3f64 overflowTime = clipCtrl->clipTime_sec - clipDuration;

			//Accounts for overflow time steps that are longer than the clip duration
			while (overflowTime > clipDuration)
			{
				overflowTime -= clipDuration;
			}

			switch (clipCtrl->clip->transitionForward[0].flag)
			{
				case a3clip_stopFlag:
					//Stop
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
					clipCtrl->clipTime_sec = clipDuration;
					clipCtrl->keyframeParam = 1.0;
					clipCtrl->clipParam = 1.0;
					return 0;
					break;
				case a3clip_playFlag: 
					//Loop
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
					clipCtrl->clipTime_sec = overflowTime;
					break;
				case a3clip_reverseFlag:
					//Impliment ping pong
					//clipCtrl->keyframeIndex = --clipCtrl->keyframeIndex;
					clipCtrl->clipTime_sec = clipDuration - overflowTime;
					//clipCtrl->playback_sec *= -(a3i32)1;
					clipCtrl->clipPool->clip->keyframeDirection *= -1;
					break;
				default:
					//Default is loop
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
					clipCtrl->clipTime_sec = overflowTime;
					break;
			}

		}
		else if (clipCtrl->clipTime_sec < 0)  //For reverse transition if clip time goes below 0
		{
			a3f64 overflowTime = -clipCtrl->clipTime_sec;

			//Accounts for overflow time steps that are longer than the clip duration
			while (overflowTime > clipDuration)
			{
				overflowTime -= clipDuration;
			}

			switch (clipCtrl->clip->transitionReverse[0].flag)
			{
				case a3clip_stopFlag:
					//Stop
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
					clipCtrl->clipTime_sec = 0;
					clipCtrl->keyframeParam = 0;
					clipCtrl->clipParam = 0;
					return 0;
					break;
				case a3clip_playFlag:
					//Loop
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
					clipCtrl->clipTime_sec = clipDuration - overflowTime;
					break;
				case a3clip_reverseFlag:
					//Impliment ping pong
					clipCtrl->clipTime_sec = (clipDuration - clipDuration) + overflowTime;
					//clipCtrl->playback_sec *= -1;
					clipCtrl->clipPool->clip->keyframeDirection *= -1;
					break;
				default:
					//Default is loop
					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
					clipCtrl->clipTime_sec = clipDuration - overflowTime;
					break;
			}
		}

		////Finds current keyframe based on transitions and updated clip time

		//Picks a starting keyframe - Tristan
		a3f64 keyFrameStartTime_T0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
		a3f64 keyFrameEndTime_T1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;

		//Makes sure it is the current keyframe Tristan created and refactored/imroved by Will and Tristan
		while (clipCtrl->clipTime_sec >= keyFrameEndTime_T1 || clipCtrl->clipTime_sec < keyFrameStartTime_T0)
		{
			clipCtrl->clipPool->clip->keyframeDirection > 0 ? clipCtrl->keyframeIndex++ : clipCtrl->keyframeIndex--;

			if ((a3i32)clipCtrl->keyframeIndex < clipCtrl->clip->keyframeCount)
			{	
				keyFrameStartTime_T0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				keyFrameEndTime_T1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
			}

			////Don't need to account for if index goes out of range here since that is done in the transitions section
			
		}

		////Gets normalized time for current keyframe and clip - Base created by Tristan and refactored/imroved by Will and Tristan
		clipCtrl->keyframeParam = (clipCtrl->clipTime_sec - keyFrameStartTime_T0) * clipCtrl->keyframe[clipCtrl->keyframeIndex].durationInv;
		clipCtrl->clipParam = clipCtrl->clip->duration_sec * clipCtrl->clip->durationInv;

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
