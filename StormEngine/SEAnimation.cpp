/******************************************************************************//**
 * \file			SEAnimation.cpp	
 * \author 			Rommel 
 * \contributions	Rommel Sim (100%)
 * \par    			zhenpengrommel.sim@digipen.edu
 *
 * \brief			Animation System
 *
 * \date   			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEAnimation.h"
#include "SEResource.h"
void SEAnimationInit()
{
	
}

void SEAnimationUpdate()
{
}

void SEAnimationExit()
{
	// empty
}

void SEPlayAnimation(std::string objectname, ACTION action)
{
	GameObject* go = SEFindObjectByName(objectname);

	if (go)
	{
		Texture* pTex = (Texture*)go->getComponent(CT_TEXTURE);
		Transform* pTrans = (Transform*)go->getComponent(CT_TRANSFORM);
		if (pTex && pTrans)
		{
			//static int i = 0; 
			//static float frame_timer = 0.0f;
			//float interval = 1.0f / pTex->Frames;
			//if (frame_timer >= interval)
			//{
			//	++i;
			//	frame_timer = 0.0f;
			//	if (i >= pTex->Frames)
			//	{
			//		i = 0;
			//	}
			//}
			//auto dt = SEGetDeltaTime();
			//frame_timer += dt * pTex->AnimationSpeed;

			Animation& pAnimate = pTex->AnimationHandler;
			int frameCount{};// = pTex->AnimationFrames.find(action)->second;
			auto find = pTex->AnimationFrames.find(action);
			if (find != pTex->AnimationFrames.end())
			{
				frameCount = find->second;
			}
			

			pAnimate.interval = 1.0f / 30.0f;
			

			while (pAnimate.frameTimer >= pAnimate.interval)
			{
				pAnimate.i++;
				pAnimate.frameTimer -= pAnimate.interval;
				/*if (pAnimate.i >= frameCount)		 // LOL!
					pAnimate.i = 0;*/
			}
			if (pAnimate.i >= frameCount)
				pAnimate.i = 0;
			const auto search = pTex->AnimationSpeed.find(action);
			if (search != pTex->AnimationSpeed.end())
			{
				pAnimate.frameTimer += (float)SEGetDeltaTime() * search->second;
			}
			std::unordered_map<int, std::vector<SEVec2 >> uv_containter = SEGetAnimationUV(objectname, action);
			std::vector<SEVec2> uv = uv_containter[pAnimate.i];
			//std::cout << "BL:" << uv[0].x << ", " << uv[0].y << std::endl;
			//std::cout << "BR:" << uv[1].x << ", " << uv[1].y << std::endl;
			//std::cout << "TR:" << uv[2].x << ", " << uv[2].y << std::endl;
			//std::cout << "TL:" << uv[3].x << ", " << uv[3].y << std::endl<< std::endl;

			TextureResource* tmpRsc; (void)tmpRsc;
			float ar = 1.0f;
			SEUint ID{};

			
			for (auto& aniTextures : pTex->AnimationTextures)
			{
				if (aniTextures.first == action)
				{
					for (auto& it : aniTextures.second)
					{
						ID = SEGetTexture(it.second);
					
							ar = GetTextureAr(it.first);
					}
				}
			}
		
			float x = pTrans->getTranslate().x;
			float y = pTrans->getTranslate().y;
			if (pTex->KeepAspectRatio)
				pTrans->setScale(pTrans->getScale().y * ar, pTrans->getScale().y);
			else
				pTrans->setScale(pTrans->getScale().x, pTrans->getScale().y);
			//float scale_y = pTrans->getScale().y;
			float angle = pTrans->getRotationAngle();

			if (!uv.empty()) 
			SEDrawTexture(x, y, pTrans->getScale().x, pTrans->getScale().y, angle, uv, ID);
		}
	}
}
