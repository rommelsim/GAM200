/******************************************************************************//**
 * \file			SESerialiser.cpp
 * \author 			Khairul	    (60%)
 * \co-author		Rommel Sim  (20%)
 * \co-author       Cheong Ming Lun (20%)
 * \par    			m.binroslan@digipen.edu
 * \par    			zhenpengrommel.sim@digipen.edu
 * \par				m.cheong@digipen.edu 
 *
 * \brief			Serialiser class for de/serialising JSON for prefabs and scenes.
 *
 * \date			January 2023

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SESerialiser.h"
#include "SELayerStack.h"
#include <ConsoleColor.h>


LEVEL GetLevelForJSON()
{
	return Level;
}

void SetLevelForJSON(const LEVEL& _level)
{
	Level = _level;
}

std::string LEVEL_STRING(LEVEL elevel)
{
	switch (elevel)
	{
	case LEVEL::LEVEL_0:
		return "Level 0";
		break;
	case LEVEL::LEVEL_1:
		return "Level 1";
		break;
	case LEVEL::LEVEL_2:
		return "Level 2";
		break;
	case LEVEL::LEVEL_3:
		return "Level 3";
		break;
	case LEVEL::MAIN_MENU:
		return "Main Menu";
		break;
	case LEVEL::SETTINGS:
		return "Settings";
		break;
	case LEVEL::EXIT:
		return "Exit Screen";
		break;
	case LEVEL::PAUSE:
		return "Pause";
		break;
	case LEVEL::WIN:
		return "Win";
		break;
	case LEVEL::LOSE:
		return "Lose";
		break;
	case LEVEL::HOW_TO_PLAY:
		return "How To Play";
		break;
	default:
		return "";
		break;
	}
}

Document SEGetJSONDocument(std::string file)
{
	std::ifstream ifs(file);
	if (!ifs)
		exit(1);

	// read and parse contents 
	std::stringstream ss;
	ss << ifs.rdbuf();

	rapidjson::Document doc;
	doc.Parse(ss.str().c_str());
	return doc;
}

void JsonDocFormat(std::string str)
{
	std::string formatstream{};

	int ind = 0;
	for (size_t i = 0; i < str.size(); i++)
	{

		if (str[i] == '}' || str[i] == ']')
		{
			ind--;
			formatstream += '\n';
			for (int it = 0; it < ind; it++)
			{
				formatstream += "    ";
			}
		}
		formatstream += str[i];

		if (str[i] == ':')
		{
			formatstream += ' ';
		}
		if (str[i] == '{' || str[i] == '[')
		{
			ind++;
			formatstream += '\n';
			for (int it = 0; it < ind; it++)
			{
				formatstream += "    ";
			}
		}

		if (str[i] == ',')
		{
			formatstream += '\n';
			for (int it = 0; it < ind; it++)
			{
				formatstream += "    ";
			}
		}
	}

	// std::cout << formatstream << std::endl;

	std::ofstream outfile("formattest.json");
	outfile << formatstream;

	std::cout << "JSON Format Applied" << std::endl;
}

std::string ID_RENDERLAYER(SERenderLayer E_RenderLayer) {
	switch (E_RenderLayer)
	{
	case SERenderLayer::BACKGROUND:
		return "Background";
		break;
	case SERenderLayer::FOREGROUND:
		return "Foreground";
		break;
	case SERenderLayer::GAMEPLAY:
		return "Gameplay";
		break;
	case SERenderLayer::OVERLAY:
		return "Overlay";
		break;
	case SERenderLayer::UI_ELEMENTS:
		return "UI_Elements";
		break;
	default:
		return "";
		break;
	}
}

void SESerialiseJSON(const std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>& container, const std::filesystem::path& out)
{
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();

	Value Parent(kArrayType);

	/*
		* To identify as a Member, put m in front of variable. (mColor)
		* To identify as an Array, put Array in front of variable. (ArrayColor)
		* To identify as an Object/Entity, put E in front of variable. (EColor)
		* To identify as a Value with a String output, put Str in front of variable. (StrTexure)
		* To identify as a Value with a Float/Int output, put F/I in front of variable. (FAngle/IAngle)
		* To identify as a Value with a Bool output, put B in front of variable. (BAnimating)
	*/

	for (const auto& scene : container)
	{
		std::string Name = scene.first;
		//LEVEL Level = scene.second.first;
		GameObject* go = scene.second.second;

		// Name of GameObject
		Value Entity(kObjectType);
		Entity.SetObject();
		Entity.AddMember("Name", Name, allocator);

		// Color Component
		Color* pColor = (Color*)go->getComponent(CT_COLOR);
		if (pColor)
		{
			float r = pColor->getColor().r;
			float g = pColor->getColor().g;
			float b = pColor->getColor().b;
			Value mColor(kObjectType);
			mColor.SetObject();

			Value ArrayColors(kArrayType);
			ArrayColors.PushBack(r, allocator);
			ArrayColors.PushBack(g, allocator);
			ArrayColors.PushBack(b, allocator);
			mColor.AddMember("RGB", ArrayColors, allocator);
			Entity.AddMember("Color", mColor, allocator);
		}

		// Transform Component
		Transform* pTransform = (Transform*)go->getComponent(CT_TRANSFORM);
		if (pTransform)
		{
			SEVec2 Scale = pTransform->getScale();
			SEVec2 Position = pTransform->getTranslate();
			SEFloat Angle = pTransform->getRotationAngle();
			SEFloat Speed = pTransform->getRotationSpeed();

			Value mTransform(kObjectType);
			mTransform.SetObject();

			Value ArrayScale(kArrayType);
			ArrayScale.PushBack(Scale.x, allocator);
			ArrayScale.PushBack(Scale.y, allocator);
			mTransform.AddMember("Scale", ArrayScale, allocator);

			mTransform.AddMember("Rotation Angle", Angle, allocator);

			mTransform.AddMember("Rotation Speed", Speed, allocator);

			Value ArrayTranslate(kArrayType);
			ArrayTranslate.PushBack(Position.x, allocator);
			ArrayTranslate.PushBack(Position.y, allocator);
			mTransform.AddMember("Translate", ArrayTranslate, allocator);

			Entity.AddMember("Transform", mTransform, allocator);
		}

		// Collision Component
		Collision* pCollision = (Collision*)go->getComponent(CT_COLLISION);
		if (pCollision)
		{
			SEFloat Min_X = pCollision->boundaries.min.x;
			SEFloat Min_Y = pCollision->boundaries.min.y;
			SEFloat Max_X = pCollision->boundaries.max.x;
			SEFloat Max_Y = pCollision->boundaries.max.y;
			SEFloat Scale_X = pCollision->boundaries.scale.x;
			SEFloat Scale_Y = pCollision->boundaries.scale.y;
			SEFloat Pos_X = pCollision->collisionPos.pos.x;
			SEFloat Pos_Y = pCollision->collisionPos.pos.y;

			Value ArrayMin(kArrayType);
			ArrayMin.PushBack(Min_X, allocator);
			ArrayMin.PushBack(Min_Y, allocator);

			Value ArrayMax(kArrayType);
			ArrayMax.PushBack(Max_X, allocator);
			ArrayMax.PushBack(Max_Y, allocator);

			Value ArrayScale(kArrayType);
			ArrayScale.PushBack(Scale_X, allocator);
			ArrayScale.PushBack(Scale_Y, allocator);

			Value ArrayPosition(kArrayType);
			ArrayPosition.PushBack(Pos_X, allocator);
			ArrayPosition.PushBack(Pos_Y, allocator);

			Value mCollision(kObjectType);
			mCollision.SetObject();
			mCollision.AddMember("Min", ArrayMin, allocator);
			mCollision.AddMember("Max", ArrayMax, allocator);
			mCollision.AddMember("Scale", ArrayScale, allocator);
			mCollision.AddMember("Position", ArrayPosition, allocator);
			Entity.AddMember("Collision", mCollision, allocator);
		}

		// Texture Component
		Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);
		if (pTexture)
		{


			Value mTexture(kObjectType);
			mTexture.SetObject();

			std::string renderPiro = std::to_string((int)pTexture->m_priority_level);

			std::string renderlayer = pTexture->GetCurrentRenderLayer();



			std::string Paths[MAX_Action_Count] = {};
			SEInt Rows[MAX_Action_Count] = {};
			SEInt Cols[MAX_Action_Count] = {};
			SEInt Frame[MAX_Action_Count] = {};
			SEInt Speed[MAX_Action_Count] = {};

			//if (isAnimating)
			//{
				// Rows
			for (const auto& texture : pTexture->Rows)
			{
				Rows[(int)texture.first] = texture.second;

			}


			// Cols
			for (const auto& texture : pTexture->Columns)
			{
				Cols[(int)texture.first] = texture.second;

			}

			// Animation Textures
			for (const auto& texture : pTexture->AnimationTextures)
			{
				for (const auto& path : texture.second)
				{

					Paths[(int)texture.first] = path.second.string();
				}
			}

			// Animation Frames
			for (const auto& texture : pTexture->AnimationFrames)
			{
				Frame[(int)texture.first] = texture.second;
			}

			// Animation Speed
			for (const auto& texture : pTexture->AnimationSpeed)
			{
				Speed[(int)texture.first] = (int)texture.second;
			}

			Value OStates(kObjectType);

			for (int i = 0; i < MAX_Action_Count; i++) {
				if (!Paths[i].empty()) {
					Value VarArray(kArrayType);

					if (i == 0) {
						VarArray.PushBack(Value{}.SetString(Paths[i].c_str(), (SizeType)Paths[i].length(), allocator), allocator);
						VarArray.PushBack(1, allocator);
						VarArray.PushBack(1, allocator);
						VarArray.PushBack(1, allocator);
						VarArray.PushBack(1.0f, allocator);
					}

					else {
						VarArray.PushBack(Value{}.SetString(Paths[i].c_str(), (SizeType)Paths[i].length(), allocator), allocator);
						VarArray.PushBack(Rows[i], allocator);
						VarArray.PushBack(Cols[i], allocator);
						VarArray.PushBack(Frame[i], allocator);
						VarArray.PushBack(Speed[i], allocator);
					}

					std::string tmpAction = ACTION_STRING((ACTION)i);

					OStates.AddMember(Value{}.SetString(tmpAction.c_str(), (SizeType)tmpAction.length(), allocator), VarArray, allocator);

				}
			}
			Value IDrender;
			IDrender.SetString(renderlayer.c_str(), (SizeType)renderlayer.length(), allocator);

			mTexture.AddMember("Render Layer", IDrender, allocator);
			mTexture.AddMember("Render Priority", renderPiro, allocator);
			mTexture.AddMember("States", OStates, allocator);
			Entity.AddMember("Texture", mTexture, allocator);
		}

		if (go->m_behaviour) {
#if _DEBUG
#if 1
			std::cout << typeid(go->m_behaviour).name() << " --> " << go->m_name << std::endl;
#endif
#endif
			std::string Behave = go->m_BehaviourName;
			Value BehaveObj(kObjectType);
			if (Behave == "Player_Behaviour" || Behave == "NPC_Behaviour") {


				BehaveObj.AddMember("Movement Speed", 0.5, allocator);

			}

			Entity.AddMember(Value{}.SetString(Behave.c_str(), (rapidjson::SizeType)Behave.length(), allocator), BehaveObj, allocator);


		}


		// Font Component
		Font* pFont = (Font*)go->getComponent(CT_FONT);
		if (pFont)
		{
			std::string FontText = pFont->m_text;
			std::string FontPath = pFont->fontPath.string();
			SEVec2 FontScale = pFont->font_scale;
			SEVec2 FontTranslate = pFont->font_translate;

			Value mFont(kObjectType);
			mFont.SetObject();

			mFont.AddMember("Text", FontText, allocator);
			mFont.AddMember("Font Path", FontPath, allocator);

			Value ArrayScale(kArrayType);
			ArrayScale.PushBack(FontScale.x, allocator);
			ArrayScale.PushBack(FontScale.y, allocator);
			mFont.AddMember("Scale", ArrayScale, allocator);

			Value ArrayTranslate(kArrayType);
			ArrayTranslate.PushBack(FontTranslate.x, allocator);
			ArrayTranslate.PushBack(FontTranslate.y, allocator);
			mFont.AddMember("Translate", ArrayTranslate, allocator);

			Entity.AddMember("Font", mFont, allocator);

		}

		Dialogue* pDialogue = (Dialogue*)go->getComponent(CT_DIALOGUE);
		if (pDialogue)
		{
			// Common Data
			std::string DialogueType;
			std::string DialogueText;  // (1) "Dialouge Text" (2) "Option Text"
			std::string ParentNode;

			// Option Data
			std::string NextDialogueName;
			int SuspicionLevel;

			// Main Object
			Value mDialogue(kObjectType);
			mDialogue.SetObject();

			switch (pDialogue->dialogue_type)
			{
			case SEDialogueType::PROMPT:
				DialogueType = "Prompt";
				DialogueText = pDialogue->dialogue_text;
				mDialogue.AddMember("Dialogue Type", DialogueType, allocator);

				break;
			case SEDialogueType::OPTION:
				DialogueType = "Option";
				NextDialogueName = pDialogue->next_node_name;
				SuspicionLevel = pDialogue->suspicion_value;

				mDialogue.AddMember("Dialogue Type", DialogueType, allocator);

				if (NextDialogueName != "")
				{
					mDialogue.AddMember("Next Dialogue Name", NextDialogueName, allocator);
				}

				mDialogue.AddMember("Suspicion Level", SuspicionLevel, allocator);
				break;
			}

			if (pDialogue->dialogue_text != "")
			{
				DialogueText = pDialogue->dialogue_text;
				mDialogue.AddMember("Dialouge Text", DialogueText, allocator);
			}

			if (pDialogue->parent_node_name != "") // if its not empty...
			{
				ParentNode = pDialogue->parent_node_name;
				mDialogue.AddMember("Parent Node", ParentNode, allocator);
			}

			Entity.AddMember("Dialogue", mDialogue, allocator);
		}

		DialogueParent* pDialogueParent = (DialogueParent*)go->getComponent(CT_DIALOGUE_PARENT);
		if (pDialogueParent)
		{
			Entity.AddMember("Dialogue Child", pDialogueParent->starting_node_name, allocator);
		}

		Parent.PushBack(Entity, allocator);

	}
	std::string Lvl = LEVEL_STRING(GetLevelForJSON());

	doc.AddMember("Level", Value{}.SetString(Lvl.c_str(), (SizeType)Lvl.length(), allocator), allocator);
	doc.AddMember("Asset", "Scene", allocator);
	doc.AddMember("Version", "1.140223", allocator);
	doc.AddMember("Game Objects", Parent, allocator);


	//rapidjson::Value Array(kArrayType);
	//Array.PushBack("First Array", allocator);
	//Array.PushBack("Second Array", allocator);
	//doc.AddMember("arrays", Array, allocator);

	// Write to Buffer (terminal)
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	doc.Accept(writer);

	// Write to SolutionDir
	std::filesystem::path solution_dir;
	std::string path = SOLUTIONDIR;
	path += out.string();
	std::ofstream ofs(path);
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer_file(osw);
	doc.Accept(writer_file);
	std::cout << "Written to: " << path << std::endl;

	// Write to Executable Dir
	std::filesystem::path exe_dir(out);
	std::ofstream ofs_exe(exe_dir.string());
	OStreamWrapper osw_exe(ofs_exe);
	Writer<OStreamWrapper> writer_file_exe(osw_exe);
	doc.Accept(writer_file_exe);
	std::cout << "Written to: " << exe_dir.string() << std::endl;
}


void SESerialiseJSON(std::vector<GameObject*>& container, std::string out)
{

	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();

	Value Parent(kArrayType);

	for (const auto& obj : container)
	{
		std::string Name = obj->m_name;
		GameObject* go = obj;

		// Name of GameObject
		Value Entity(kObjectType);
		Entity.SetObject();
		Entity.AddMember("Name", Name, allocator);

		// Color Component
		Color* pColor = (Color*)go->getComponent(CT_COLOR);
		if (pColor)
		{
			float r = pColor->getColor().r;
			float g = pColor->getColor().g;
			float b = pColor->getColor().b;
			Value mColor(kObjectType);
			mColor.SetObject();

			Value ArrayColors(kArrayType);
			ArrayColors.PushBack(r, allocator);
			ArrayColors.PushBack(g, allocator);
			ArrayColors.PushBack(b, allocator);
			mColor.AddMember("RGB", ArrayColors, allocator);
			Entity.AddMember("Color", mColor, allocator);
		}

		// Transform Component
		Transform* pTransform = (Transform*)go->getComponent(CT_TRANSFORM);
		if (pTransform)
		{
			SEVec2 Scale = pTransform->getScale();
			SEVec2 Position = pTransform->getTranslate();
			SEFloat Angle = pTransform->getRotationAngle();
			SEFloat Speed = pTransform->getRotationSpeed();

			Value mTransform(kObjectType);
			mTransform.SetObject();

			Value ArrayScale(kArrayType);
			ArrayScale.PushBack(Scale.x, allocator);
			ArrayScale.PushBack(Scale.y, allocator);
			mTransform.AddMember("Scale", ArrayScale, allocator);

			mTransform.AddMember("Rotation Angle", Angle, allocator);

			mTransform.AddMember("Rotation Speed", Speed, allocator);

			Value ArrayTranslate(kArrayType);
			ArrayTranslate.PushBack(Position.x, allocator);
			ArrayTranslate.PushBack(Position.y, allocator);
			mTransform.AddMember("Translate", ArrayTranslate, allocator);

			Entity.AddMember("Transform", mTransform, allocator);
		}

		// Collision Component
		Collision* pCollision = (Collision*)go->getComponent(CT_COLLISION);
		if (pCollision)
		{
			SEFloat Min_X = pCollision->boundaries.min.x;
			SEFloat Min_Y = pCollision->boundaries.min.y;
			SEFloat Max_X = pCollision->boundaries.max.x;
			SEFloat Max_Y = pCollision->boundaries.max.y;
			SEFloat Scale_X = pCollision->boundaries.scale.x;
			SEFloat Scale_Y = pCollision->boundaries.scale.y;
			SEFloat Pos_X = pCollision->collisionPos.pos.x;
			SEFloat Pos_Y = pCollision->collisionPos.pos.y;

			Value ArrayMin(kArrayType);
			ArrayMin.PushBack(Min_X, allocator);
			ArrayMin.PushBack(Min_Y, allocator);

			Value ArrayMax(kArrayType);
			ArrayMax.PushBack(Max_X, allocator);
			ArrayMax.PushBack(Max_Y, allocator);

			Value ArrayScale(kArrayType);
			ArrayScale.PushBack(Scale_X, allocator);
			ArrayScale.PushBack(Scale_Y, allocator);

			Value ArrayPosition(kArrayType);
			ArrayPosition.PushBack(Pos_X, allocator);
			ArrayPosition.PushBack(Pos_Y, allocator);

			Value mCollision(kObjectType);
			mCollision.SetObject();
			mCollision.AddMember("Min", ArrayMin, allocator);
			mCollision.AddMember("Max", ArrayMax, allocator);
			mCollision.AddMember("Scale", ArrayScale, allocator);
			mCollision.AddMember("Position", ArrayPosition, allocator);
			Entity.AddMember("Collision", mCollision, allocator);
		}

		// Texture Component
		Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);
		if (pTexture)
		{
			Value mTexture(kObjectType);
			mTexture.SetObject();

			SERenderLayer renderLayer = pTexture->m_render_layer;


			std::string renderPiro = std::to_string((int)pTexture->m_priority_level);

			std::string renderlayer = pTexture->GetCurrentRenderLayer();
			std::string renderpriority = std::to_string(pTexture->m_priority_level);



			std::string Paths[MAX_Action_Count] = {};
			SEInt Rows[MAX_Action_Count] = {};
			SEInt Cols[MAX_Action_Count] = {};
			SEInt Frame[MAX_Action_Count] = {};
			SEInt Speed[MAX_Action_Count] = {};

			//if (isAnimating)
			//{
				// Rows
			for (const auto& texture : pTexture->Rows)
			{
				Rows[(int)texture.first] = texture.second;

				//switch (texture.first)
				//{
				//case ACTION::ACTION_IDLE:
				//	IdleRows = texture.second;
				//	break;
				//case ACTION::ACTION_WALKING:
				//	WalkingRows = texture.second;
				//	break;
				//case ACTION::ACTION_CLEANING:
				//	CleaningRows = texture.second;
				//	break;
				//case ACTION::ACTION_ALERT:
				//	AlertRows = texture.second;
				//	break;
				//default: break;
				//}


			}


			// Cols
			for (const auto& texture : pTexture->Columns)
			{
				Cols[(int)texture.first] = texture.second;

				//switch (texture.first)
				//{
				//case ACTION::ACTION_IDLE:
				//	IdleCols = texture.second;
				//	break;
				//case ACTION::ACTION_WALKING:
				//	WalkingCols = texture.second;
				//	break;
				//case ACTION::ACTION_CLEANING:
				//	CleaningCols = texture.second;
				//	break;
				//case ACTION::ACTION_ALERT:
				//	AlertCols = texture.second;
				//	break;
				//default: break;
				//}
			}

			// Animation Textures
			for (const auto& texture : pTexture->AnimationTextures)
			{
				for (const auto& path : texture.second)
				{

					Paths[(int)texture.first] = path.second.string();

					//switch (texture.first)
					//{
					//case ACTION::ACTION_IDLE:
					//	IdlePath = path.second.string();
					//	break;
					//case ACTION::ACTION_WALKING:
					//	WalkingPath = path.second.string();
					//	break;
					//case ACTION::ACTION_CLEANING:
					//	CleaningPath = path.second.string();
					//	break;
					//case ACTION::ACTION_ALERT:
					//	AlertPath = path.second.string();
					//	break;
					//default: break;
					//}
				}
			}

			// Animation Frames
			for (const auto& texture : pTexture->AnimationFrames)
			{
				Frame[(int)texture.first] = texture.second;

				//switch (texture.first)
				//{
				//case ACTION::ACTION_IDLE:
				//	IdleFrames = texture.second;
				//	break;
				//case ACTION::ACTION_WALKING:
				//	WalkingFrames = texture.second;
				//	break;
				//case ACTION::ACTION_CLEANING:
				//	CleaningFrames = texture.second;
				//	break;
				//case ACTION::ACTION_ALERT:
				//	AlertFrames = texture.second;
				//	break;
				//default: break;
				//}
			}

			// Animation Speed
			for (const auto& texture : pTexture->AnimationSpeed)
			{
				Speed[(int)texture.first] = (int)texture.second;

				//switch (texture.first)
				//{
				//case ACTION::ACTION_IDLE:
				//	IdleSpeed = texture.second;
				//	break;
				//case ACTION::ACTION_WALKING:
				//	WalkingSpeed = texture.second;
				//	break;
				//case ACTION::ACTION_CLEANING:
				//	CleaningSpeed = texture.second;
				//	break;
				//case ACTION::ACTION_ALERT:
				//	AlertSpeed = texture.second;
				//	break;
				//default: break;
				//}
			}

			Value OStates(kObjectType);

			for (int i = 0; i < MAX_Action_Count; i++) {
				if (!Paths[i].empty()) {
					Value VarArray(kArrayType);

					if (i == 0) {
						VarArray.PushBack(Value{}.SetString(Paths[i].c_str(), (SizeType)Paths[i].length(), allocator), allocator);
						VarArray.PushBack(1, allocator);
						VarArray.PushBack(1, allocator);
						VarArray.PushBack(1, allocator);
						VarArray.PushBack(1.0f, allocator);
					}

					else {
						VarArray.PushBack(Value{}.SetString(Paths[i].c_str(), (SizeType)Paths[i].length(), allocator), allocator);
						VarArray.PushBack(Rows[i], allocator);
						VarArray.PushBack(Cols[i], allocator);
						VarArray.PushBack(Frame[i], allocator);
						VarArray.PushBack(Speed[i], allocator);
					}

					std::string tmpAction = ACTION_STRING((ACTION)i);

					OStates.AddMember(Value{}.SetString(tmpAction.c_str(), (SizeType)tmpAction.length(), allocator), VarArray, allocator);

					/*switch ((ACTION)i)
					{
					case ACTION::ACTION_NULL:
						OStates.AddMember("Null", VarArray, allocator);
						break;
					case ACTION::ACTION_IDLE:
						OStates.AddMember("Idle", VarArray, allocator);
						break;
					case ACTION::ACTION_WALKING:
						OStates.AddMember("Walking", VarArray, allocator);
						break;
					case ACTION::ACTION_CLEANING:
						OStates.AddMember("Cleaning", VarArray, allocator);
						break;
					case ACTION::ACTION_ALERT:
						OStates.AddMember("Alert", VarArray, allocator);
						break;
					case ACTION::ACTION_GLOW:
						OStates.AddMember("Glowing", VarArray, allocator);
						break;
					case ACTION::ACTION_SEARCHING:
						OStates.AddMember("Search", VarArray, allocator);
						break;
					case ACTION::ACTION_BUTTON_ISHOVERED:
						OStates.AddMember("Mouse Hovering", VarArray, allocator);
						break;
					case ACTION::ACTION_BUTTON_IDLE:
						OStates.AddMember("Mouse Idle", VarArray, allocator);
						break;
					case ACTION::ACTION_CLEANING_BROOM:
						OStates.AddMember("Cleaning Broom", VarArray, allocator);
						break;
					case ACTION::ACTION_IDLE_BROOM:
						OStates.AddMember("Idle Broom", VarArray, allocator);
						break;
					case ACTION::ACTION_WALKING_BROOM:
						OStates.AddMember("Walking Broom", VarArray, allocator);
						break;
					case ACTION::ACTION_IDLE_PAPER:
						OStates.AddMember("Idle Paper", VarArray, allocator);
						break;
					case ACTION::ACTION_WALKING_PAPER:
						OStates.AddMember("Walk Paper", VarArray, allocator);
						break;
					case ACTION::ACTION_IDLE_PHOTO:
						OStates.AddMember("Idle Photo", VarArray, allocator);
						break;
					case ACTION::ACTION_WALKING_PHOTO:
						OStates.AddMember("Walk Photo", VarArray, allocator);
						break;
					case ACTION::ACTION_IDLE_TRASHBAG:
						OStates.AddMember("Idle Trashbag", VarArray, allocator);
						break;
					case ACTION::ACTION_WALKING_TRASHBAG:
						OStates.AddMember("Walk Trashbag", VarArray, allocator);
						break;
					default:
						break;
					}*/

				}
			}
			mTexture.AddMember("Render Layer", ID_RENDERLAYER(renderLayer), allocator);
			mTexture.AddMember("Render Priority", renderPiro, allocator);
			mTexture.AddMember("States", OStates, allocator);
			Entity.AddMember("Texture", mTexture, allocator);

			/*if (go->m_behaviour) {
#if _DEBUG
#if 1
				std::cout << typeid(go->m_behaviour).name() << " --> " << go->m_name << std::endl;
#endif
#endif
				std::string Behave = go->m_BehaviourName;
				Value BehaveObj(kObjectType);
				if (Behave == "Player_Behaviour" || Behave == "NPC_Behaviour") {


					BehaveObj.AddMember("Movement Speed", 0.5, allocator);

				}

				Entity.AddMember(Value{}.SetString(Behave.c_str(), Behave.length(), allocator), BehaveObj, allocator);


			}*/

		}

		// Font Component
		Font* pFont = (Font*)go->getComponent(CT_FONT);
		if (pFont)
		{
			std::string FontPath = pFont->fontPath.string();

			Value mFont(kObjectType);
			mFont.AddMember("Font Path", FontPath, allocator);
			Entity.AddMember("Font", mFont, allocator);
		}

		Parent.PushBack(Entity, allocator);

	}
	doc.AddMember("Asset", "Prefab", allocator);
	doc.AddMember("Version", "1.140223", allocator);
	doc.AddMember("Game Objects", Parent, allocator);

	// Write to Buffer (terminal)
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	doc.Accept(writer);


	// Write to Solution Directory, for compiling.
	std::filesystem::path solution_dir;
	std::string path = SOLUTIONDIR;
	path += out;
	std::ofstream ofs(path);
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer_file(osw);
	doc.Accept(writer_file);
	std::cout << "Written to: " << path << std::endl;

	// Write to current directory, for UXGD to save files. 
	std::ofstream ofs_exe(out);
	OStreamWrapper osw_exe(ofs_exe);
	Writer<OStreamWrapper> writer_file_exe(osw_exe);
	doc.Accept(writer_file_exe);
	std::cout << "Written to: " << out << std::endl;
}

void TextureComponentMember(const Value& States, const std::string& str, Texture* texture)
{
	ACTION tmp_action{};
	if (str == "Null") tmp_action = ACTION::ACTION_NULL;
	else if (str == "Idle") tmp_action = ACTION::ACTION_IDLE;
	else if (str == "Walking") tmp_action = ACTION::ACTION_WALKING;
	else if (str == "Cleaning Broom") tmp_action = ACTION::ACTION_CLEANING_BROOM;
	else if (str == "Idle Broom") tmp_action = ACTION::ACTION_IDLE_BROOM;
	else if (str == "Walking Broom") tmp_action = ACTION::ACTION_WALKING_BROOM;
	else if (str == "Idle Paper") tmp_action = ACTION::ACTION_IDLE_PAPER;
	else if (str == "Walk Paper") tmp_action = ACTION::ACTION_WALKING_PAPER;
	else if (str == "Idle Photo") tmp_action = ACTION::ACTION_IDLE_PHOTO;
	else if (str == "Walk Photo") tmp_action = ACTION::ACTION_WALKING_PHOTO;
	else if (str == "Idle Trashbag") tmp_action = ACTION::ACTION_IDLE_TRASHBAG;
	else if (str == "Walk Trashbag") tmp_action = ACTION::ACTION_WALKING_TRASHBAG;
	else if (str == "Search") tmp_action = ACTION::ACTION_SEARCHING;
	else if (str == "SearchHorn") tmp_action = ACTION::ACTION_SEARCH_FAKE_HORN;
	else if (str == "Alert") tmp_action = ACTION::ACTION_ALERT;
	else if (str == "Glowing") tmp_action = ACTION::ACTION_GLOW;
	else if (str == "Mouse Hovering") tmp_action = ACTION::ACTION_BUTTON_ISHOVERED;
	else if (str == "Mouse Idle") tmp_action = ACTION::ACTION_BUTTON_IDLE;
	else if (str == "Active Real") tmp_action = ACTION::ACTION_ACTIVE_REAL;
	else if (str == "Active Fake") tmp_action = ACTION::ACTION_ACTIVE_FAKE;
	else if (str == "Active Glass") tmp_action = ACTION::ACTION_ACTIVE_GLASS;
	else if (str == "Active TrashBag") tmp_action = ACTION::ACTION_ACTIVE_TRASHBAG;
	else if (str == "Active") tmp_action = ACTION::ACTION_ACTIVE;
	else if (str == "Inactive") tmp_action = ACTION::ACTION_INACTIVE;
	else if (str == "Sus_Triggered") tmp_action = ACTION::ACTION_GLOW;
	else if (str == "Exclamation Mark") tmp_action = ACTION::ACTION_EXCLAMATION;
	else if (str == "Poofing") tmp_action = ACTION::ACTION_POOFING;
	else if (str == "Objective Close") tmp_action = ACTION::ACTION_OBJECTIVE_CLOSE;
	else if (str == "Objective Open") tmp_action = ACTION::ACTION_OBJECTIVE_OPEN;
	else if (str == "Lose") tmp_action = ACTION::ACTION_LOSE_SCREEN;

	// Level 2 Animations
	else if (str == "Idle Mop") tmp_action = ACTION::ACTION_IDLE_MOP;
	else if (str == "Walk Mop") tmp_action = ACTION::ACTION_WALKING_MOP;
	else if (str == "Cleaning Mop") tmp_action = ACTION::ACTION_CLEANING_MOP;
	else if (str == "Idle Bloody Mop") tmp_action = ACTION::ACTION_IDLE_BLOODY_MOP;
	else if (str == "Walking Bloody Mop") tmp_action = ACTION::ACTION_WALKING_BLOODY_MOP;
	else if (str == "Washing") tmp_action = ACTION::ACTION_WASHING;
	else if (str == "Bucket Full") tmp_action = ACTION::ACTION_BUCKET_FULL;
	else if (str == "Idle Full Paint Bucket") tmp_action = ACTION::ACTION_IDLE_FULL_PAINT_BUCKET;
	else if (str == "Fake Blood") tmp_action = ACTION::ACTION_FAKE_BLOOD;
	else if (str == "Bucket Empty") tmp_action = ACTION::ACTION_BUCKET_EMPTY;
	else if (str == "Walking Full Paint Bucket") tmp_action = ACTION::ACTION_WALKING_FULL_PAINT_BUCKET;
	else if (str == "Idle Empty Paint Bucket") tmp_action = ACTION::ACTION_IDLE_EMPTY_PAINT_BUCKET;
	else if (str == "Walking Empty Paint Bucket") tmp_action = ACTION::ACTION_WALKING_EMPTY_PAINT_BUCKET;
	else if (str == "Pouring Paint") tmp_action = ACTION::ACTION_POURING_PAINT;
	else if (str == "Idle Bloody Knife") tmp_action = ACTION::ACTION_IDLE_BLOODY_KNIFE;
	else if (str == "Walking Bloody Knife") tmp_action = ACTION::ACTION_WALKING_BLOODY_KNIFE;
	else if (str == "Idle Clean Knife") tmp_action = ACTION::ACTION_IDLE_CLEAN_KNIFE;
	else if (str == "Walking Clean Knife") tmp_action = ACTION::ACTION_WALKING_CLEAN_KNIFE;
	else if (str == "Clean Knife") tmp_action = ACTION::ACTION_CLEANED_KNIFE;
	else if (str == "Real Horn") tmp_action = ACTION::ACTION_REAL_HORN;
	else if (str == "Fake Horn") tmp_action = ACTION::ACTION_FAKE_HORN;
	else if (str == "Idle Candy Horn") tmp_action = ACTION::ACTION_IDLE_CANDY_HORN;
	else if (str == "Walking Candy Horn") tmp_action = ACTION::ACTION_WALKING_CANDY_HORN;
	else if (str == "Idle Candy Corn") tmp_action = ACTION::ACTION_IDLE_FAKE_CANDY_CORN;
	else if (str == "Walking Candy Corn") tmp_action = ACTION::ACTION_WALKING_FAKE_CANDY_CORN;
	else if (str == "Jack Winding") tmp_action = ACTION::ACTION_JACK_IN_BOX_WINDING;



	std::filesystem::path TexturePath = States[str][0].GetString();
	const int row = States[str][1].GetInt();
	const int col = States[str][2].GetInt();
	const int frames = States[str][3].GetInt();
	const float speed = States[str][4].GetFloat();

	std::unordered_map<std::string, std::filesystem::path> tmp_pair{ std::make_pair(TexturePath.filename().string(), TexturePath) };
	texture->AnimationTextures.emplace(tmp_action, tmp_pair);

	texture->Rows.emplace(std::make_pair(tmp_action, row));
	texture->Columns.emplace(std::make_pair(tmp_action, col));
	texture->AnimationFrames.emplace(std::make_pair(tmp_action, frames));
	texture->AnimationSpeed.emplace(std::make_pair(tmp_action, speed));

	// Compute UVs
	texture->AnimationUV.emplace(std::make_pair(tmp_action, texture->SEComputeUV(row, col, frames)));
	SELoadTexture(TexturePath);
}



//// used for loading scene
//void SE
// ialiseJSON(std::string file, SEBool dialogue_stuff)
//{
//	//// Clear all previous objects first
//	if (!dialogue_stuff)
//	{
//		SEDeleteAllObject();
//		SEDeleteEverything();
//	}


//keep this. I foresee changes in serialisation
void SEDeserialiseJSON(std::string file) {
	//SEClearScene();
	Document base = SEGetJSONDocument(file);
	const Value& versionJ = base["Version"];
	std::string version = versionJ.GetString();

	if ((version != "1.0" && version != "1.140223"))
	{
		printf("JSON Version error. Version is not recognized\n");
		exit(EXIT_FAILURE);
	}
	if (version == "1.0") {
#if _DEBUG
		std::cout << red << "DESER(file): v1.0 detected..." << white << std::endl;
#endif
		//SEDeserialiseJSON_v1_0(file);
	}

	else if (version == "1.140223") {
#if _DEBUG
		std::cout << green << "DESER(file): v1.140223 detected..." << white << std::endl;
#endif
		Level = SEDeserialiseJSON_v1_140223(file);
		return;
	}

}


//mainly used in scene loading
LEVEL SEDeserialiseJSON_v1_140223(std::string fileSEDeserialiseJSON)
{
	std::unique_ptr<Color> color;
	std::unique_ptr<Transform> transform;
	std::unique_ptr<Collision> collision;
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Font> font;
	std::unique_ptr<Dialogue> dialogue;
	std::unique_ptr<DialogueParent> dial_parent;

	Document doc = SEGetJSONDocument(fileSEDeserialiseJSON);
	LEVEL location{};

	const Value& JSON_Level = doc["Level"];
	const Value& JSON_Asset = doc["Asset"]; (void)JSON_Asset;
	const Value& JSON_Ver = doc["Version"]; (void)JSON_Ver;
	const Value& JSONValue_Current = doc["Game Objects"];
	std::string _Level = JSON_Level.GetString();


	// Data to be parsed into Game Object creation
	std::string name{};
	std::string tag;
	SEBool isActive;
	float r, g, b;
	SEVec2 Scale, Position;
	float RotAngle, RotSpeed;
	SEVec2 collision_min{}, collision_max{}, collision_scale{}, collision_pos{};
	std::filesystem::path TexturePath_IDLE{}, TexturePath_WALK{}, TexturePath_CLEAN{}, TexturePath_ALERT{}, FontPath, StaticTexturePath; // Added 1/7/2023 - for [StaticTexturePath] : static textures
	SEBool Animating{};
	SEInt Frames_IDLE{}, Frames_WALK{}, Frames_CLEAN{}; (void)Frames_WALK; (void)Frames_IDLE; (void)Frames_CLEAN;
	SEFloat Speed_IDLE{}, Speed_WALK{}, Speed_CLEAN{}; (void)Speed_WALK; (void)Speed_IDLE; (void)Speed_CLEAN;

	// Data required for [Dialogue] component. 
	// ----------------------------------------------------
	// std::string name{} -> this marks the name of the [Game Object], also it marks as an identifier for linked dialogues. 
	std::string dialogue_type;				    // [1] Prompt or Option or Boarder
	std::string dialogue_text;				    // [2] "Text to display" - can be used for all [Prompt] & [Option]
	SEInt suspicion_level;						// [3] Suspicion Level
	std::string option_next_node;				// [4] Name of the next dialogue to link to.
	std::string parent_node_name;				// [5] Name of the node that these dialogue elements belong to.

	SEVec2 font_scale;							// [1] Font Independent Scaling
	SEVec2 font_translate;						// [2] Font Independent Translation

	//std::string dialogue_type;					// [2] Dialogue Type - indicates if this "gameObject" is a [Prompt] / [Option]
	//std::string dialogue_text;					// [3] Text buffer for both [Prompt] & [Option] text. 

	//SEBool suspicion_flag;						// [4] Suspicious or not?
	//SEInt suspicion_level;						// [5] Suspicion Level (how much suspicious points to raise)
	//std::string option_next_node;				    // [6] Name of the next dialogue to link to. 
	//std::string parent_prompt;					// [7] Which Prompt does this option belong to?

	//std::string boarder_belongs_to;

	// Get All components from a game object in current scene...
	SizeType size = JSONValue_Current.Size();
	for (SizeType i = 0; i < size; ++i)
	{
		std::unique_ptr<GameObject> go;

		if (JSONValue_Current[i].IsObject())
		{
			// Check if members exist!
			auto NameBool = JSONValue_Current[i].HasMember("Name");
			auto TagBool = JSONValue_Current[i].HasMember("Tag");			// Added 1/17/2023
			auto IsActiveBool = JSONValue_Current[i].HasMember("IsActive"); // Added 2/16/2023 - in place of "isRendering" flag as fonts do not contain a <Texture> component. (generalize this flag to all objects) 
			auto ColorBool = JSONValue_Current[i].HasMember("Color");
			auto TransformBool = JSONValue_Current[i].HasMember("Transform");
			auto CollisionBool = JSONValue_Current[i].HasMember("Collision");
			auto TextureBool = JSONValue_Current[i].HasMember("Texture");
			auto FontBool = JSONValue_Current[i].HasMember("Font");
			auto PlayerBehaviourBool = JSONValue_Current[i].HasMember("Player_Behaviour");
			auto NPCBehaviourBool = JSONValue_Current[i].HasMember("NPC_Behaviour");
			auto ButtonBehaviourBool = JSONValue_Current[i].HasMember("Button_Behaviour");
			auto EvidenceBehaviourBool = JSONValue_Current[i].HasMember("Evidence_Behaviour");
			auto UIBehaviourBool = JSONValue_Current[i].HasMember("UI_Behaviour");
			auto SusMeterBool = JSONValue_Current[i].HasMember("SusMeter_Behaviour");	(void)SusMeterBool;
			auto ProximityBool = JSONValue_Current[i].HasMember("Proximity_Behaviour");
			auto ArrowBool = JSONValue_Current[i].HasMember("Arrow_Behaviour");
			auto DialogueBool = JSONValue_Current[i].HasMember("Dialogue");
			auto DialogueParentBool = JSONValue_Current[i].HasMember("Dialogue Child");
			auto EsclamationBool = JSONValue_Current[i].HasMember("Esclamation_Behaviour");
			auto SplashBool = JSONValue_Current[i].HasMember("Splash_Behaviour");
			auto CreditsBool = JSONValue_Current[i].HasMember("Credits_Behaviour");

			if (NameBool)
			{
				name = JSONValue_Current[i]["Name"].GetString();
				go = std::make_unique<GameObject>(name);
				std::string wall_pre = name.substr(0, 4);
				if (wall_pre == "Wall")
					go->setTag("Wall");
			}

			// Added 1/17/2023
			if (TagBool)
			{
				tag = JSONValue_Current[i]["Tag"].GetString();
				go->setTag(tag);
			}

			// Added 2/16/2023
			if (IsActiveBool)
			{
				isActive = JSONValue_Current[i]["IsActive"].GetBool();
				go->isActive = isActive;
			}

			if (ColorBool) {
				const Value& ColorComp = JSONValue_Current[i]["Color"];
				r = ColorComp["RGB"][0].GetFloat();
				g = ColorComp["RGB"][1].GetFloat();
				b = ColorComp["RGB"][2].GetFloat();
				color = std::make_unique<Color>();
				color->setColor({ r,g,b });
				go->addComponent(color.release());
			}
			if (TransformBool)
			{	
				const Value& TransformComp = JSONValue_Current[i]["Transform"];
				Scale = { TransformComp["Scale"][0].GetFloat(), TransformComp["Scale"][1].GetFloat() };
				RotAngle = TransformComp["Rotation Angle"].GetFloat();
				RotSpeed = TransformComp["Rotation Speed"].GetFloat();
				Position = { TransformComp["Translate"][0].GetFloat(), TransformComp["Translate"][1].GetFloat() };
				transform = std::make_unique<Transform>();
				transform->setScale(Scale);
				transform->setRotationAngle(RotAngle);
				if (RotSpeed)
				{
					transform->setRotateFlag(1);
					transform->setRotationSpeed(RotSpeed);
				}
				transform->setScale(Scale);
				transform->setTranslate(Position);

				// Added 3/8/2023 (for reseting)
				transform->m_starting_translate = Position;
				transform->m_starting_scale = Scale;

				go->addComponent(transform.release());

			}
			if (CollisionBool)
			{
				const Value& CollisionComp = JSONValue_Current[i]["Collision"];
				collision_min = { CollisionComp["Min"][0].GetFloat(), CollisionComp["Min"][1].GetFloat() };
				collision_max = { CollisionComp["Max"][0].GetFloat() ,CollisionComp["Max"][1].GetFloat() };
				collision_scale = { CollisionComp["Scale"][0].GetFloat(), CollisionComp["Scale"][1].GetFloat() };
				collision_pos = { CollisionComp["Position"][0].GetFloat(), CollisionComp["Position"][1].GetFloat() };
				collision = std::make_unique<Collision>();
				collision->SetCollision(collision_pos, collision_scale);
				collision->isActive = true;
				go->addComponent(collision.release());
				// todo: why need to namespace ?
			}
			if (TextureBool)
			{
				const Value& TextureComp = JSONValue_Current[i]["Texture"];
				//Animating = TextureComp["Animating"].GetBool();
				texture = std::make_unique<Texture>();


				if (TextureComp.HasMember("Render Off"))
				{
					texture->isRendering = false;
				}
				else
				{
					texture->isRendering = true; // for both dialogue font and boarder and other gameobjects
				}

				// Get Path, Speed and Frames.
				const Value& States = TextureComp["States"];

				if (States.HasMember("Null"))				TextureComponentMember(States, "Null", texture.get());
				if (States.HasMember("Idle"))				TextureComponentMember(States, "Idle", texture.get());
				if (States.HasMember("Walking"))			TextureComponentMember(States, "Walking", texture.get());
				if (States.HasMember("Cleaning Broom"))		TextureComponentMember(States, "Cleaning Broom", texture.get());
				if (States.HasMember("Idle Broom"))			TextureComponentMember(States, "Idle Broom", texture.get());
				if (States.HasMember("Walking Broom"))		TextureComponentMember(States, "Walking Broom", texture.get());
				if (States.HasMember("Idle Paper"))			TextureComponentMember(States, "Idle Paper", texture.get());
				if (States.HasMember("Walk Paper"))			TextureComponentMember(States, "Walk Paper", texture.get());
				if (States.HasMember("Idle Photo"))			TextureComponentMember(States, "Idle Photo", texture.get());
				if (States.HasMember("Walk Photo"))			TextureComponentMember(States, "Walk Photo", texture.get());
				if (States.HasMember("Idle Trashbag"))		TextureComponentMember(States, "Idle Trashbag", texture.get());
				if (States.HasMember("Walk Trashbag"))		TextureComponentMember(States, "Walk Trashbag", texture.get());
				if (States.HasMember("Search"))				TextureComponentMember(States, "Search", texture.get());
				if (States.HasMember("SearchHorn"))				TextureComponentMember(States, "SearchHorn", texture.get());
				if (States.HasMember("Mouse Idle"))			TextureComponentMember(States, "Mouse Idle", texture.get());
				if (States.HasMember("Mouse Hovering"))		TextureComponentMember(States, "Mouse Hovering", texture.get());
				if (States.HasMember("Alert"))				TextureComponentMember(States, "Alert", texture.get());
				if (States.HasMember("Active Real"))		TextureComponentMember(States, "Active Real", texture.get());
				if (States.HasMember("Active Fake"))		TextureComponentMember(States, "Active Fake", texture.get());
				if (States.HasMember("Active Glass"))		TextureComponentMember(States, "Active Glass", texture.get());
				if (States.HasMember("Active TrashBag"))	TextureComponentMember(States, "Active TrashBag", texture.get());
				if (States.HasMember("Active"))				TextureComponentMember(States, "Active", texture.get());
				if (States.HasMember("Inactive"))			TextureComponentMember(States, "Inactive", texture.get());
				if (States.HasMember("Exclamation Mark"))	TextureComponentMember(States, "Exclamation Mark", texture.get());
				if (States.HasMember("Objective Close"))	TextureComponentMember(States, "Objective Close", texture.get());
				if (States.HasMember("Objective Open"))		TextureComponentMember(States, "Objective Open", texture.get());
				if (States.HasMember("Glowing"))			TextureComponentMember(States, "Glowing", texture.get());
				if (States.HasMember("Lose"))				TextureComponentMember(States, "Lose", texture.get());
				if (States.HasMember("Idle Mop"))		    TextureComponentMember(States, "Idle Mop", texture.get());
				if (States.HasMember("Walk Mop"))			TextureComponentMember(States, "Walk Mop", texture.get());
				if (States.HasMember("Cleaning Mop"))		TextureComponentMember(States, "Cleaning Mop", texture.get());
				if (States.HasMember("Idle Bloody Mop"))	TextureComponentMember(States, "Idle Bloody Mop", texture.get());
				if (States.HasMember("Walking Bloody Mop"))	TextureComponentMember(States, "Walking Bloody Mop", texture.get());
				if (States.HasMember("Washing"))			TextureComponentMember(States, "Washing", texture.get());
				if (States.HasMember("Bucket Full"))	    TextureComponentMember(States, "Bucket Full", texture.get());
				if (States.HasMember("Idle Full Paint Bucket"))	TextureComponentMember(States, "Idle Full Paint Bucket", texture.get());
				if (States.HasMember("Fake Blood"))			TextureComponentMember(States, "Fake Blood", texture.get());
				if (States.HasMember("Bucket Empty"))		TextureComponentMember(States, "Bucket Empty", texture.get());
				if (States.HasMember("Walking Full Paint Bucket"))	TextureComponentMember(States, "Walking Full Paint Bucket", texture.get());
				if (States.HasMember("Idle Empty Paint Bucket"))	TextureComponentMember(States, "Idle Empty Paint Bucket", texture.get());
				if (States.HasMember("Walking Empty Paint Bucket"))	TextureComponentMember(States, "Walking Empty Paint Bucket", texture.get());
				if (States.HasMember("Idle Bloody Knife"))			TextureComponentMember(States, "Idle Bloody Knife", texture.get());
				if (States.HasMember("Walking Bloody Knife"))	    TextureComponentMember(States, "Walking Bloody Knife", texture.get());
				if (States.HasMember("Idle Clean Knife"))			TextureComponentMember(States, "Idle Clean Knife", texture.get());
				if (States.HasMember("Walking Clean Knife"))	    TextureComponentMember(States, "Walking Clean Knife", texture.get());
				if (States.HasMember("Clean Knife"))				TextureComponentMember(States, "Clean Knife", texture.get());
				if (States.HasMember("Fake Horn"))					TextureComponentMember(States, "Fake Horn", texture.get());
				if (States.HasMember("Real Horn"))				    TextureComponentMember(States, "Real Horn", texture.get());
				if (States.HasMember("Idle Candy Horn"))			TextureComponentMember(States, "Idle Candy Horn", texture.get());
				if (States.HasMember("Walking Candy Horn"))			TextureComponentMember(States, "Walking Candy Horn", texture.get());
				if (States.HasMember("Idle Candy Corn"))			TextureComponentMember(States, "Idle Candy Corn", texture.get());
				if (States.HasMember("Walking Candy Corn"))			TextureComponentMember(States, "Walking Candy Corn", texture.get());
				if (States.HasMember("Pouring Paint"))				TextureComponentMember(States, "Pouring Paint", texture.get());
				if (States.HasMember("Jack Winding"))				TextureComponentMember(States, "Jack Winding", texture.get());
				if (States.HasMember("Poofing"))					TextureComponentMember(States, "Poofing", texture.get());


				



				std::string render_layer = TextureComp["Render Layer"].GetString();

				texture->SEAssignJSONtoRenderLayer(render_layer); // assigns the enum layer based on what have been de-serialized under "Render Layer" tag. 

				texture->m_priority_level = std::stoi(TextureComp["Render Priority"].GetString()); // assigns priority level within the same layer.

				texture->IsAnimating = Animating;
				go->addComponent(texture.release());
			}

			if (FontBool)
			{
				const Value& FontComp = JSONValue_Current[i]["Font"];
				FontPath = FontComp["Font Path"].GetString();

				if (FontComp.HasMember("Text")) // Added 16/2/2023 (for dialogue text because the dialogue text is tagged to the <Dialogue> Component>
				{
					std::string text = FontComp["Text"].GetString();
					font = std::make_unique<Font>(FontPath, text);				// added text for Font component.
				}

				else
				{
					font = std::make_unique<Font>(FontPath);
				}

				//float font_scale_x = FontComp["Scale"][0].GetFloat();
				//float font_scale_y = FontComp["Scale"][1].GetFloat();
				font_scale = { FontComp["Scale"][0].GetFloat(), FontComp["Scale"][1].GetFloat() };
				font->font_scale = font_scale;

				font_translate = { FontComp["Translate"][0].GetFloat(), FontComp["Translate"][1].GetFloat() };
				font->font_translate = font_translate;

				go->addComponent(font.release());
			}

			if (DialogueBool)
			{
				const Value& DialogueComp = JSONValue_Current[i]["Dialogue"];
				dialogue = std::make_unique<Dialogue>();

				if (DialogueComp.HasMember("Dialogue Type"))
				{
					dialogue_type = DialogueComp["Dialogue Type"].GetString(); // retrieves string to determine the which dialogue component this is (Prompt/Option/Boarder)
				}

				if (dialogue_type == "Prompt")
				{
					dialogue->dialogue_type = SEDialogueType::PROMPT;

				}


				else if (dialogue_type == "Option")
				{
					dialogue->dialogue_type = SEDialogueType::OPTION;

					if (DialogueComp.HasMember("Option Text"))
					{
						dialogue_text = DialogueComp["Option Text"].GetString();
						dialogue->dialogue_text = dialogue_text;
					}

					if (DialogueComp.HasMember("Next Dialogue Name"))
					{
						option_next_node = DialogueComp["Next Dialogue Name"].GetString();
						dialogue->next_node_name = option_next_node;
					}

					if (DialogueComp.HasMember("Suspicion Level"))
					{
						suspicion_level = DialogueComp["Suspicion Level"].GetInt();
						dialogue->suspicion_value = suspicion_level;
					}
					else
					{
						dialogue->suspicion_value = 0;
					}
				}

				else if (dialogue_type == "Boarder")
				{
					dialogue->dialogue_type = SEDialogueType::BOARDER;
				}

				else if (dialogue_type == "Thought Bubble")
				{
					dialogue->dialogue_type = SEDialogueType::THOUGHT_BUBBLE;
				}

				else if (dialogue_type == "NPC Name")
				{
					dialogue->dialogue_type = SEDialogueType::NPC_NAME;
				}

				else if (dialogue_type == "Miscellaneous") // esclamation mark or anything need to be translated above a gameObject.
				{
					dialogue->dialogue_type = SEDialogueType::MISCELLEANEOUS;
				}

				if (DialogueComp.HasMember("Parent NPC"))
				{
					dialogue->parent_npc = DialogueComp["Parent NPC"].GetString();
				}

				// Common Checks for everyone.
				if (DialogueComp.HasMember("Dialogue Text"))
				{
					dialogue_text = DialogueComp["Dialogue Text"].GetString();
					dialogue->dialogue_text = dialogue_text;
				}

				if (DialogueComp.HasMember("Parent Node"))
				{
					const Value& ParentNode = DialogueComp["Parent Node"];
					for (unsigned int k = 0; k < ParentNode.Size(); k++)
					{
						parent_node_name = DialogueComp["Parent Node"][k].GetString();
						dialogue->parent_node_names.push_back(parent_node_name);
					}


				}

				go->addComponent(dialogue.release());

			}

			std::string DialogueChild;

			if (DialogueParentBool) // if this gameObject owns a <Dialogue> ...
			{
				const Value& DialogueComp = JSONValue_Current[i]["Dialogue Child"];
				dial_parent = std::make_unique<DialogueParent>();

				for (unsigned int j = 0; j < DialogueComp.Size(); j++)
				{
					DialogueChild = DialogueComp[j].GetString();
					dial_parent->starting_node_names.push_back(DialogueChild);
				}

				go->addComponent(dial_parent.release());
			}

			if (EvidenceBehaviourBool)
			{
				go->m_behaviour = std::make_unique<Behaviour_Evidence>(go.get());
				go->m_BehaviourName = "Evidence_Behaviour";
				go->m_behaviour->Init();

			}
			if (PlayerBehaviourBool)
			{
				go->m_behaviour = std::make_unique<Behaviour_Player>(go.get());
				go->m_BehaviourName = "Player_Behaviour";
				go->m_behaviour->Init();
				Behaviour_Player* tmp = (Behaviour_Player*)go->m_behaviour.get();

				const Value& behaviour = JSONValue_Current[i]["Player_Behaviour"];
				tmp->m_movementSpeed = behaviour["Movement Speed"].GetFloat();
			}

			if (NPCBehaviourBool)
			{
				go->m_behaviour = std::make_unique<Behaviour_NPC>(go.get());
				go->m_BehaviourName = "NPC_Behaviour";
				go->m_behaviour->Init();
				Behaviour_NPC* tmp = (Behaviour_NPC*)go->m_behaviour.get();

				const Value& behaviour = JSONValue_Current[i]["NPC_Behaviour"];
				tmp->m_movementSpeed = behaviour["Movement Speed"].GetFloat();

				if (behaviour.HasMember("Chase Zone"))
				{
					tmp->chase_zone_name = behaviour["Chase Zone"].GetString(); // save a reference of the "chase zone" object name for access purposes. 
				}
			}

			if (ButtonBehaviourBool)
			{
				go->m_behaviour = std::make_unique<Behaviour_Button>(go.get());
				go->m_BehaviourName = "Button_Behaviour";
				go->m_behaviour->Init();

			}

			if (UIBehaviourBool) {
				go->m_behaviour = std::make_unique<Behaviour_GameplayUI>(go.get());
				go->m_BehaviourName = "UI_Behaviour";
				go->m_behaviour->Init();
			}

			if (ArrowBool)
			{
				go->m_behaviour = std::make_unique<Behaviour_Arrow>(go.get());
				go->m_BehaviourName = "Arrow_Behaviour";
				go->m_behaviour->Init();
			}
			
			if (ProximityBool)
			{

				go->m_behaviour = std::make_unique<Behaviour_Proximity>(go.get());
				go->m_BehaviourName = "Proximity_Behaviour";

				const Value& ProximityComp = JSONValue_Current[i]["Proximity_Behaviour"];
				std::string npc_name;

				if (ProximityComp.HasMember("NPC Name"))
				{
					npc_name = ProximityComp["NPC Name"].GetString();
					dynamic_cast<Behaviour_Proximity*>(go->m_behaviour.get())->npc_name = npc_name;
					go->m_behaviour->Init();
				}

			}

			if (EsclamationBool)
			{
				go->m_behaviour = std::make_unique<Behaviour_Esclamation>(go.get()); 
				go->m_BehaviourName = "Esclamation_Behaviour";

				const Value& EsclamationComp = JSONValue_Current[i]["Esclamation_Behaviour"];
				std::string npc_name;

				if (EsclamationComp.HasMember("NPC Name"))
				{
					npc_name = EsclamationComp["NPC Name"].GetString();
					dynamic_cast<Behaviour_Esclamation*>(go->m_behaviour.get())->npc_name = npc_name;
				}
			}

			if (SplashBool) {
				go->m_behaviour = std::make_unique<Behaviour_Splash>(go.get());
				go->m_BehaviourName = "Splash_Behaviour";
				go->m_behaviour->Init();
			}

			if (CreditsBool) {
				go->m_behaviour = std::make_unique<Behaviour_Credits>(go.get());
				go->m_BehaviourName = "Splash_Behaviour";
				go->m_behaviour->Init();
			}
			

			if (_Level == "Main Menu")
				location = LEVEL::MAIN_MENU;
			else if (_Level == "Win")
				location = LEVEL::WIN;
			else if (_Level == "Lose")
				location = LEVEL::LOSE;
			else if (_Level == "Pause")
				location = LEVEL::PAUSE;
			else if (_Level == "How To Play")
				location = LEVEL::HOW_TO_PLAY;
			else if (_Level == "Exit Screen")
				location = LEVEL::EXIT;
			else if (_Level == "Settings")
				location = LEVEL::SETTINGS;
			else if (_Level == "Level 1")
				location = LEVEL::LEVEL_1;
			else if (_Level == "Cutscene")
				location = LEVEL::CUTSCENE;
			else if (_Level == "Ending")
				location = LEVEL::ENDING;
			else if (_Level == "Splash")
				location = LEVEL::SPLASH;
			else if (_Level == "Credits")
				location = LEVEL::CREDITS;


			go->m_level = location;
			SECreateObject(location, go.release());
		}
	}
	return location;
}
//keep this, I foresee changes in the serialisation
void SEDeserialiseJSON(std::vector<GameObject*>& container, std::string file) {
	Document base = SEGetJSONDocument(file);
	const Value& versionJ = base["Version"];
	std::string version = versionJ.GetString();

	if (version == "1.0") {
#if _DEBUG
		std::cout << red << "DESER(GO+file): v1.0 detected..." << white << std::endl;
#endif
		//SEDeserialiseJSON_v1_0(container, file);
	}

	else if (version == "1.140223") {
#if _DEBUG
		std::cout << green << "DESER(GO+file): v1.140223 detected..." << white << std::endl;
#endif
		SEDeserialiseJSON_v1_140223(container, file);
	}

	if ((version != "1.0" && version != "1.140223")) {
		printf("JSON Version error. Version is not recognized\n");
		exit(EXIT_FAILURE);
	}
}

//mainly used for prefabs
void SEDeserialiseJSON_v1_140223(std::vector<GameObject*>& container, std::string file) {
	std::unique_ptr<Color> color;
	std::unique_ptr<Transform> transform;
	std::unique_ptr<Collision> collision;
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Font> font;


	Document doc = SEGetJSONDocument(file);
	const Value& JSONValue_Current = doc["Game Objects"];
	//const std::filesystem::path Current_Scene = doc["Current Scene"].GetString();

	// Get Current Scene Level Indicator...
	char result_current{};
	/*for (auto& ch : Current_Scene.string())
	{
		if (ch >= '0' && ch <= '9')
		{
			result_current = ch;
			break;
		}
	}*/
	const LEVEL Current = static_cast<LEVEL>(result_current - '0');

	//// Get Next Scene Level Indicator...
	//const std::filesystem::path Next_Level = doc["Next Scene"].GetString();
	//char result_next{};
	//for (auto& ch : Next_Level.string())
	//{
	//	if (ch >= '0' && ch <= '9')
	//	{
	//		result_next = ch;
	//		break;
	//	}
	//}
	//const LEVEL Next = static_cast<LEVEL>(result_next - '0');


	// Data to be parsed into Game Object creation
	std::string name{};
	float r, g, b;
	SEVec2 Scale, Position;
	float RotAngle, RotSpeed;
	SEVec2 collision_min{}, collision_max{}, collision_scale{}, collision_pos{};
	std::filesystem::path TexturePath_IDLE{}, TexturePath_WALK{}, TexturePath_CLEAN{}, TexturePath_ALERT{}, FontPath, StaticTexturePath; // Added 1/7/2023 - for [StaticTexturePath] : static textures
	SEBool Animating{};
	SEInt Frames_IDLE{}, Frames_WALK{}, Frames_CLEAN{}; (void)Frames_WALK; (void)Frames_IDLE; (void)Frames_CLEAN;
	SEFloat Speed_IDLE{}, Speed_WALK{}, Speed_CLEAN{}; (void)Speed_WALK; (void)Speed_IDLE; (void)Speed_CLEAN;

	// Get All components from a game object in current scene...
	SizeType size = JSONValue_Current.Size();
	for (SizeType i = 0; i < size; ++i)
	{
		std::unique_ptr<GameObject> go;
		if (JSONValue_Current[i].IsObject())
		{

			// Check if members exist!
			auto NameBool = JSONValue_Current[i].HasMember("Name");
			auto ColorBool = JSONValue_Current[i].HasMember("Color");
			auto TransformBool = JSONValue_Current[i].HasMember("Transform");
			auto CollisionBool = JSONValue_Current[i].HasMember("Collision");
			auto TextureBool = JSONValue_Current[i].HasMember("Texture");
			auto FontBool = JSONValue_Current[i].HasMember("Font");
			auto PlayerBehaviour = JSONValue_Current[i].HasMember("Behaviour");
			//auto DialogueBool = JSONValue_Current[i].HasMember("Dialogue");

			if (NameBool)
			{
				name = JSONValue_Current[i]["Name"].GetString();
				go = std::make_unique<GameObject>(name);
			}
			if (ColorBool) {
				const Value& ColorComp = JSONValue_Current[i]["Color"];
				r = ColorComp["RGB"][0].GetFloat();
				g = ColorComp["RGB"][1].GetFloat();
				b = ColorComp["RGB"][2].GetFloat();
				color = std::make_unique<Color>();
				color->setColor({ r,g,b });
				go->addComponent(color.release());
			}
			if (TransformBool)
			{
				const Value& TransformComp = JSONValue_Current[i]["Transform"];
				Scale = { TransformComp["Scale"][0].GetFloat(), TransformComp["Scale"][1].GetFloat() };
				RotAngle = TransformComp["Rotation Angle"].GetFloat();
				RotSpeed = TransformComp["Rotation Speed"].GetFloat();
				Position = { TransformComp["Translate"][0].GetFloat(), TransformComp["Translate"][1].GetFloat() };
				transform = std::make_unique<Transform>();
				transform->setScale(Scale);
				transform->setRotationAngle(RotAngle);
				if (RotSpeed)
				{
					transform->setRotateFlag(1);
					transform->setRotationSpeed(RotSpeed);
				}
				transform->setScale(Scale);
				transform->setTranslate(Position);
				go->addComponent(transform.release());
			}
			if (CollisionBool)
			{
				const Value& CollisionComp = JSONValue_Current[i]["Collision"];
				collision_min = { CollisionComp["Min"][0].GetFloat(), CollisionComp["Min"][1].GetFloat() };
				collision_max = { CollisionComp["Max"][0].GetFloat() ,CollisionComp["Max"][0].GetFloat() };
				collision_scale = { CollisionComp["Scale"][0].GetFloat(), CollisionComp["Scale"][0].GetFloat() };
				collision_pos = { CollisionComp["Position"][0].GetFloat(), CollisionComp["Position"][0].GetFloat() };
				collision = std::make_unique<Collision>();
				go->addComponent(collision.release());
			}
			if (TextureBool)
			{
				const Value& TextureComp = JSONValue_Current[i]["Texture"];
				texture = std::make_unique<Texture>();

				// Get Path, Speed and Frames.
				const Value& States = TextureComp["States"];


				if (States.HasMember("Null"))				TextureComponentMember(States, "Null", texture.get());
				if (States.HasMember("Idle"))				TextureComponentMember(States, "Idle", texture.get());
				if (States.HasMember("Walking"))			TextureComponentMember(States, "Walking", texture.get());
				if (States.HasMember("Cleaning Broom"))		TextureComponentMember(States, "Cleaning Broom", texture.get());
				if (States.HasMember("Idle Broom"))			TextureComponentMember(States, "Idle Broom", texture.get());
				if (States.HasMember("Walking Broom"))		TextureComponentMember(States, "Walking Broom", texture.get());
				if (States.HasMember("Idle Paper"))			TextureComponentMember(States, "Idle Paper", texture.get());
				if (States.HasMember("Walk Paper"))			TextureComponentMember(States, "Walk Paper", texture.get());
				if (States.HasMember("Idle Photo"))			TextureComponentMember(States, "Idle Photo", texture.get());
				if (States.HasMember("Walk Photo"))			TextureComponentMember(States, "Walk Photo", texture.get());
				if (States.HasMember("Idle Trashbag"))		TextureComponentMember(States, "Idle Trashbag", texture.get());
				if (States.HasMember("Walk Trashbag"))		TextureComponentMember(States, "Walk Trashbag", texture.get());
				if (States.HasMember("Search"))				TextureComponentMember(States, "Search", texture.get());
				if (States.HasMember("SearchHorn"))				TextureComponentMember(States, "SearchHorn", texture.get());
				if (States.HasMember("Mouse Idle"))			TextureComponentMember(States, "Mouse Idle", texture.get());
				if (States.HasMember("Mouse Hovering"))		TextureComponentMember(States, "Mouse Hovering", texture.get());
				if (States.HasMember("Alert"))				TextureComponentMember(States, "Alert", texture.get());
				if (States.HasMember("Active Real"))		TextureComponentMember(States, "Active Real", texture.get());
				if (States.HasMember("Active Fake"))		TextureComponentMember(States, "Active Fake", texture.get());
				if (States.HasMember("Active Glass"))		TextureComponentMember(States, "Active Glass", texture.get());
				if (States.HasMember("Active TrashBag"))	TextureComponentMember(States, "Active TrashBag", texture.get());
				if (States.HasMember("Active"))				TextureComponentMember(States, "Active", texture.get());
				if (States.HasMember("Inactive"))			TextureComponentMember(States, "Inactive", texture.get());
				if (States.HasMember("Exclamation Mark"))	TextureComponentMember(States, "Exclamation Mark", texture.get());
				if (States.HasMember("Objective Close"))	TextureComponentMember(States, "Objective Close", texture.get());
				if (States.HasMember("Objective Open"))		TextureComponentMember(States, "Objective Open", texture.get());
				if (States.HasMember("Idle Mop"))		    TextureComponentMember(States, "Idle Mop", texture.get());
				if (States.HasMember("Idle Bloody Knife"))  TextureComponentMember(States, "Idle Bloody Knife", texture.get());


				std::string render_layer = TextureComp["Render Layer"].GetString();

				texture->SEAssignJSONtoRenderLayer(render_layer); // assigns the enum layer based on what have been de-serialized under "Render Layer" tag. 

				texture->m_priority_level = std::stoi(TextureComp["Render Priority"].GetString()); // assigns priority level within the same layer.

				texture->IsAnimating = Animating;
				texture->isRendering = true;
				go->addComponent(texture.release());
			}
			if (FontBool)
			{
				const Value& FontComp = JSONValue_Current[i]["Font"];
				FontPath = FontComp["Font Path"].GetString();

				if (FontComp.HasMember("Text"))
				{
					std::string text = FontComp["Text"].GetString();
					font = std::make_unique<Font>(FontPath, text);
				}

				else
				{
					font = std::make_unique<Font>(FontPath, "");
				}


				go->addComponent(font.release());
			}

			if (PlayerBehaviour)
			{
				go->m_behaviour = std::make_unique<Behaviour_Player>(go.get());
				Behaviour_Player* tmp = (Behaviour_Player*)go->m_behaviour.get();
				tmp->m_health = JSONValue_Current[i]["Player_Behaviour"].GetInt();
				tmp->m_movementSpeed = JSONValue_Current[i]["Movement Speed"].GetFloat();

			}

			container.push_back(go.release());
		}
	}
}

std::vector<std::string> SEGetAllTexturesFrom(std::string path)
{
	std::vector<std::string> tmp{};

	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(path))
		tmp.push_back(dirEntry.path().string());

	return tmp;
}

Editor_Configuration SEReadEditorConfig(std::string input_file)
{
	Editor_Configuration e_Config{};
	Document ConfigDoc = SEGetJSONDocument(input_file);
	const Value& EditorCurr = ConfigDoc["Editor"];

	for (auto i = 0; i < (int)EditorCurr.Size(); i++)
	{
		if (EditorCurr[i].IsObject())
		{
			std::string Sys = EditorCurr[i]["System"].GetString();

			if (Sys == "Graphics")
			{
				const Value& Window = EditorCurr[i]["Window"];
				const Value& Performance = EditorCurr[i]["Performance"];
				const Value& Theme = EditorCurr[i]["Theme"];

				if (Window.IsObject())
				{
					e_Config.Window_title = Window["Title"].GetString();
					//if (Window["Fullscreen"].GetString() == "ON") {
					if(std::strcmp(Window["Fullscreen"].GetString(), "ON") == 0)
					{
						e_Config.Window_fullscreen = true;
					}

					//if (Window["Fullscreen"].GetString() == "OFF") {
					if (std::strcmp(Window["Fullscreen"].GetString(), "OFF") == 0)
					{
						e_Config.Window_fullscreen = false;
					}

					e_Config.Window_width = Window["Width"].GetInt();
					e_Config.Window_height = Window["Height"].GetInt();
				}

				else
				{
				}

				if (Performance.IsObject())
				{
					e_Config.Performance_fps = Performance["FPS"].GetInt();
					e_Config.Performance_profiler = Performance["Profiler"].GetString();
				}

				else
				{
				}

				if (Theme.IsObject())//extra
				{
					Theme["Type"].GetString();
					Theme["Current"].GetString();
				}

				else
				{
				}
			}

			if (Sys == "Audio")
			{
				const Value& BGM = EditorCurr[i]["BGM"];
				const Value& Assets = EditorCurr[i]["Assets"];

				if (BGM.IsObject())
				{
					e_Config.Audio_State = BGM["State"].GetString();
					e_Config.Audio_BGMName = BGM["Name"].GetString();
					e_Config.Audio_Volume = BGM["Volume"].GetInt();
				}

				else
				{
				}

				if (Assets.IsObject())
				{
					Assets["State"].GetString();
					Assets["Volume"].GetInt();
				}

				else
				{
				}
			}
		}
	}

	return e_Config;
}

void SEWriteEditorConfig(std::string output_file)
{
	Document TestDoc;
	TestDoc.SetObject();
	Document::AllocatorType& alloc = TestDoc.GetAllocator();

	Value Base(kArrayType);

	Value Object(kObjectType);
	Object.SetObject();
	Object.AddMember("System", "Graphics", alloc);

	Value Comp(kObjectType);
	Comp.SetObject();
	Comp.AddMember("Title", "StormEngine", alloc);
	Comp.AddMember("Fullscreen", "OFF", alloc);
	Comp.AddMember("Width", 1440, alloc);
	Comp.AddMember("Height", 810, alloc);
	Object.AddMember("Window", Comp, alloc);

	Comp.SetObject();
	Comp.AddMember("FPS", 60, alloc);
	Comp.AddMember("Profiler", "ON", alloc);
	Object.AddMember("Performance", Comp, alloc);

	Comp.SetObject();
	Comp.AddMember("Type", "Preset", alloc);
	Comp.AddMember("Current", "Hurricane Blue", alloc);
	Object.AddMember("Theme", Comp, alloc);

	Base.PushBack(Object, alloc);

	Object.SetObject();
	Object.AddMember("System", "Audio", alloc);

	Comp.SetObject();
	Comp.AddMember("State", "ON", alloc);
	Comp.AddMember("Name", "BGM_ROCK", alloc);
	Comp.AddMember("Volume", 20, alloc);
	Object.AddMember("BGM", Comp, alloc);

	Comp.SetObject();
	Comp.AddMember("State", "ON", alloc);
	Comp.AddMember("Volume", 20, alloc);
	Object.AddMember("Assets", Comp, alloc);

	Base.PushBack(Object, alloc);

	TestDoc.AddMember("Editor", Base, alloc);

	StringBuffer strBuff;
	Writer<StringBuffer> testWriter(strBuff);
	TestDoc.Accept(testWriter);

	std::ofstream out(output_file);
	OStreamWrapper ostw(out);
	Writer<OStreamWrapper> filescribe(ostw);
	TestDoc.Accept(filescribe);
}

Game_Configuration SEReadGameConfig(std::string input_file)
{
	Game_Configuration g_config;

	Document ConfigDoc = SEGetJSONDocument(input_file);
	const Value& GameCurr = ConfigDoc["Game"];

	for (auto i = 0; i < (int)GameCurr.Size(); i++)
	{
		if (GameCurr[i].IsObject())
		{
			std::string sys = GameCurr[i]["System"].GetString();

			if (sys == "Graphics")
			{
				const Value& Window = GameCurr[i]["Window"];
				const Value& Performance = GameCurr[i]["Performance"];

				if (Window.IsObject())
				{
					g_config.Window_title = Window["Title"].GetString();
					g_config.Window_fullscreen = Window["Fullscreen"].GetString();
					g_config.Window_width = Window["Width"].GetInt();
					g_config.Window_height = Window["Height"].GetInt();
				}

				else
				{
				}

				if (Performance.IsObject())
				{
					g_config.Performance_fps = Performance["FPS"].GetInt();
				}

				else
				{
				}
			}

			if (sys == "Audio")
			{
				const Value& Volumes = GameCurr[i]["Volumes"];

				if (Volumes.IsObject())
				{
					g_config.BGM_Volume = Volumes["BGM"].GetInt();
					g_config.SFX_Volume = Volumes["SFX"].GetInt();
				}

				else
				{
				}
			}
		}
	}
	return g_config;
}

void SEWriteGameConfig(std::string output_file)
{
	Document TestDoc;
	TestDoc.SetObject();
	Document::AllocatorType& alloc = TestDoc.GetAllocator();

	Value Base(kArrayType);

	Value Object(kObjectType);
	Object.SetObject();
	Object.AddMember("System", "Graphics", alloc);

	Value Comp(kObjectType);
	Comp.SetObject();
	Comp.AddMember("Title", "Game Name", alloc);
	Comp.AddMember("Fullscreen", "OFF", alloc);
	Comp.AddMember("Width", 1440, alloc);
	Comp.AddMember("Height", 810, alloc);
	Object.AddMember("Window", Comp, alloc);

	Comp.SetObject();
	Comp.AddMember("FPS", 60, alloc);
	Object.AddMember("Performance", Comp, alloc);

	Base.PushBack(Object, alloc);

	Object.SetObject();
	Object.AddMember("System", "Audio", alloc);

	Comp.SetObject();
	Comp.AddMember("BGM", 20, alloc);
	Comp.AddMember("SFX", 20, alloc);
	Object.AddMember("Volumes", Comp, alloc);

	Base.PushBack(Object, alloc);

	TestDoc.AddMember("Game", Base, alloc);

	StringBuffer strBuff;
	Writer<StringBuffer> testWriter(strBuff);
	TestDoc.Accept(testWriter);

	std::ofstream out(output_file);
	OStreamWrapper ostw(out);
	Writer<OStreamWrapper> filescribe(ostw);
	TestDoc.Accept(filescribe);
}

std::vector<std::string> SEReadObjectiveList(std::filesystem::path filepath) {
	std::vector<std::string> out;
	Document doc = SEGetJSONDocument(filepath.generic_string());
	std::string _Level = {};
	if (doc.HasMember("Level")) {
		_Level = doc["Level"].GetString();
	}

	else
		_Level = "None";

	if (doc.HasMember("Objectives") && _Level != "None") {
		const Value& ObjectiveArr = doc["Objectives"];
		out.push_back(ObjectiveArr[0].GetString());
		out.push_back(ObjectiveArr[1].GetString());
		out.push_back(ObjectiveArr[2].GetString());
	}


	return out;

}

std::vector<std::string> SEReadList(std::filesystem::path filepath)
{
	std::vector<std::string> list;


	Document listdoc = SEGetJSONDocument(filepath.generic_string());

	if (listdoc.HasMember("Help List")) {
		const Value& Help = listdoc["Help List"];
		for (rapidjson::SizeType i = 0; i < Help.Capacity(); i++) {
			list.push_back(Help[i].GetString());
		}
	}

	return list;
}
