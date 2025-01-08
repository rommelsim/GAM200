/******************************************************************************//**
 * \file			SEFactory.cpp
 * \author 			Rommel Sim (100%)
 * \par    			zhenpengrommel.sim@digipen.edu
 *
 * \brief			Factory 
 *
 * \date   			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEFactory.h"
#include "SESerialiser.h"

std::unordered_map<std::string, GameObject*> gameObjectArray;
extern LEVEL current_level;

std::string ACTION_STRING(ACTION eAction) {

	switch (eAction)
	{
	case ACTION::ACTION_NULL:
		return "Null";
		break;
	case ACTION::ACTION_IDLE:
		return "Idle";
		break;
	case ACTION::ACTION_WALKING:
		return "Walking";
		break;
	case ACTION::ACTION_CLEANING:
		return "Cleaning";
		break;
	case ACTION::ACTION_ALERT:
		return "Alert";
		break;
	case ACTION::ACTION_GLOW:
		return "Glowing";
		break;
	case ACTION::ACTION_SEARCHING:
		return "Search";
		break;
	case ACTION::ACTION_SEARCH_FAKE_HORN:
		return "SearchHorn";
		break;
	case ACTION::ACTION_BUTTON_ISHOVERED:
		return "Mouse Hovering";
		break;
	case ACTION::ACTION_BUTTON_IDLE:
		return "Mouse Idle";
		break;
	case ACTION::ACTION_CLEANING_BROOM:
		return "Cleaning Broom";
		break;
	case ACTION::ACTION_IDLE_BROOM:
		return "Idle Broom";
		break;
	case ACTION::ACTION_WALKING_BROOM:
		return "Walking Broom";
		break;
	case ACTION::ACTION_IDLE_PAPER:
		return "Idle Paper";
		break;
	case ACTION::ACTION_WALKING_PAPER:
		return "Walk Paper";
		break;
	case ACTION::ACTION_IDLE_PHOTO:
		return "Idle Photo";
		break;
	case ACTION::ACTION_WALKING_PHOTO:
		return "Walk Photo";
		break;
	case ACTION::ACTION_IDLE_TRASHBAG:
		return "Idle Trashbag";
		break;
	case ACTION::ACTION_WALKING_TRASHBAG:
		return "Walk Trashbag";
		break;
	case ACTION::ACTION_ACTIVE_REAL:
		return "Active Real";
		break;
	case ACTION::ACTION_ACTIVE_FAKE:
		return "Active Fake";
		break;
	case ACTION::ACTION_ACTIVE_GLASS:
		return "Active Glass";
		break;
	case ACTION::ACTION_ACTIVE_TRASHBAG:
		return "Active TrashBag";
		break;
	case ACTION::ACTION_OBJECTIVE_OPEN:
		return "Objective Open";
		break;
	case ACTION::ACTION_OBJECTIVE_CLOSE:
		return "Objective Close";
		break;
	case ACTION::ACTION_INACTIVE:
		return "Inactive";
		break;
	case ACTION::ACTION_EXCLAMATION:
		return "Exclamation Mark";
		break;
	case ACTION::ACTION_POOFING:
		return "Poofing";
		break;
	case ACTION::ACTION_ENUM_COUNT:
		return "ENUM COUNT";
		break;
	default: 
		return "";
		break;
	}

}

Component::~Component() {
	//PRINT("Component destroyed\n"); 
}

Color::Color() : m_color(1.0f, 1.0f, 1.0f)
{
	compType = COMPONENT_TYPE::CT_COLOR;
}

void Color::setColor(const SEVec3& c) { m_color = c; }

const SEVec3& Color::getColor() { return m_color; }

Transform::Transform()
{
	compType = COMPONENT_TYPE::CT_TRANSFORM;

	// Create prefabs for scale rot trasn
	m_scale = SEVec2(100.0f, 100.0f);							// default size
	m_rotate_angle = 0.0f;
	m_rotate_speed = 0.0f;
	m_rotate = { m_rotate_angle, m_rotate_speed };
	m_translate = SEVec2(0.0f, 0.0f);						// For now, default pos!
	if (m_rotate_speed > 0.0f)
		m_rotate_flag = 1;
	else
		m_rotate_flag = 0;
}

void Transform::setScale(const SEFloat& multiplier) { m_scale = { multiplier, multiplier }; }

void Transform::setScale(const SEFloat& multiplier_x, const SEFloat& multiplier_y) { m_scale = { multiplier_x, multiplier_y }; }

void Transform::setScale(const SEVec2& scale) { m_scale = { scale }; }

void Transform::setRotationAngle(const SEFloat& angle) { m_rotate_angle = { angle }; }

void Transform::setRotationSpeed(const SEFloat& speed) { m_rotate_speed = { speed }; }

void Transform::setTranslate(const SEVec2& trans) { m_translate = { trans }; }

void Transform::setModelTransform(const SEMat3& mdl_transform) { m_model_transform = { mdl_transform }; }

void Transform::setModelToNdcTransform(const SEMat3& mdl_to_ndc_transform) { m_model_to_ndc_transform = { mdl_to_ndc_transform }; }

void Transform::setAABBToNdcTransform(const SEMat3& AABB_to_ndc_transform) { m_AABB_to_ndc_transform = { AABB_to_ndc_transform }; }

void Transform::setRotateFlag(const SEBool& flag) { m_rotate_flag = { flag }; }

const SEVec2& Transform::getScale() { return m_scale; }

const SEFloat& Transform::getRotationSpeed() { return m_rotate_speed; }

const SEFloat& Transform::getRotationAngle() { return m_rotate_angle; }
const SEVec2& Transform::getTranslate() { return m_translate; }

const SEMat3& Transform::getModelTransform() { return m_model_transform; }

const SEMat3& Transform::getModelToNdcTransform() { return m_model_to_ndc_transform; }

const SEMat3& Transform::getAABBToNdcTransform() { return m_AABB_to_ndc_transform; }

const SEBool& Transform::getRotateFlag() { return m_rotate_flag; }

Collision::Collision()
{
	compType = COMPONENT_TYPE::CT_COLLISION;
	collisionType = StormEngine::COLLISION_TYPE::SQUARE;

	// generate aabb data from objectTransform
	boundaries.min = StormEngine::Vec2(-10.0f, -10.0f);
	boundaries.max = StormEngine::Vec2(10.0f, 10.0f);

	boundaries.scale = StormEngine::Vec2(10.0f, 10.0f);;

	collisionPos.pos = glm::vec2(.0f, .0f);
}

Collision::~Collision()
{
}

//bool Collision::CollisionCheck(glm::vec2 point_other)
//{
//
//	StormEngine::AABB exBounds = StormEngine::GenerateBoundingBox(point_other);
//
//	if (StormEngine::Collision_Rect(this->boundaries, exBounds))
//	{
//		//std::cout << "case THIS to POINT collision\n";
//		return true;
//	}
//
//	return false;
//}
//
//bool Collision::CollisionCheck(Collision* pos_other)
//{
//	if (this)
//	{
//		if (StormEngine::Collision_Rect(this->boundaries, pos_other->boundaries))
//		{
//			//std::cout << "case THIS to BOX collision\n";
//			return true;
//		}
//	}
//	return false;
//}
//
//bool Collision::CollisionCheck(Collision* pos1, Collision* pos2)
//{
//	if (StormEngine::Collision_Rect(pos1->boundaries, pos2->boundaries))
//	{
//		//std::cout << "case BOX to BOX collision\n";
//		return true;
//	}
//
//	return false;
//}
//
//bool Collision::CollisionCheck(Collision* pos1, glm::vec2 pos2)
//{
//	StormEngine::AABB exBounds = StormEngine::GenerateBoundingBox(pos2);
//
//	// Not necessary to create a box to point check 
//	//	(only mouse using this at the moment)
//	if (StormEngine::Collision_Rect(pos1->boundaries, exBounds))
//	{
//		//std::cout << "case BOX to POINT collision\n";
//		return true;
//	}
//
//	return false;
//}

StormEngine::CollisionDir Collision::CollisionDirCheck(Collision* pos_other)
{
	StormEngine::CollisionDir Direction = StormEngine::CollisionRectDir(this->boundaries, pos_other->boundaries);

	return Direction;
}

void Collision::SetCollision(glm::vec2 pos, glm::vec2 size)
{
	collisionPos.pos = pos;

	boundaries.min = StormEngine::Vec2(pos.x - size.x, pos.y - size.y);
	boundaries.max = StormEngine::Vec2(pos.x + size.x, pos.y + size.y);

	boundaries.scale = StormEngine::Vec2(size.x, size.y);
}

void Collision::SetCollisionSize(glm::vec2 size)
{
	boundaries.min = StormEngine::Vec2(collisionPos.pos.x - size.x, collisionPos.pos.y - size.y);
	boundaries.max = StormEngine::Vec2(collisionPos.pos.x + size.x, collisionPos.pos.y + size.y);

	boundaries.scale = StormEngine::Vec2(size.x, size.y);
}

bool SECheckCollision(Collision* pos1, Collision* pos2)
{
	if (StormEngine::Collision_Rect(pos1->boundaries, pos2->boundaries))
	{
		//std::cout << "case BOX to BOX collision\n";
		return true;
	}

	return false;
}
bool SECheckCollision(Collision* pos1, glm::vec2 pos2)
{
	StormEngine::AABB exBounds = StormEngine::GenerateBoundingBox(pos2);

	// Not necessary to create a box to point check 
	//	(only mouse using this at the moment)
	if (StormEngine::Collision_Rect(pos1->boundaries, exBounds))
	{
		//std::cout << "case BOX to POINT collision\n";
		return true;
	}

	return false;
}


Texture::Texture()
{
	compType = COMPONENT_TYPE::CT_TEXTURE;

}

Texture::Texture(std::filesystem::path texture_path, bool _isAnimated, SERenderLayer render_layer, int render_priority) : m_texture_path(texture_path), IsAnimating(_isAnimated), m_priority_level(render_priority)
{
	(void)render_layer;
	compType = COMPONENT_TYPE::CT_TEXTURE;
	SELoadTexture(texture_path);
	m_texture_data = SEGetTexture(texture_path);
	isRendering = true;
}

void Texture::SEAssignJSONtoRenderLayer(std::string layer)
{
	if (layer == "Background")
	{
		m_render_layer = SERenderLayer::BACKGROUND;
	}
	else if (layer == "Gameplay")
	{
		m_render_layer = SERenderLayer::GAMEPLAY;
	}

	else if (layer == "Foreground")
	{
		m_render_layer = SERenderLayer::FOREGROUND;
	}

	else if (layer == "UI Elements")
	{
		m_render_layer = SERenderLayer::UI_ELEMENTS;
	}

	else if (layer == "Overlay")
	{
		m_render_layer = SERenderLayer::OVERLAY;
	}

	else
	{
		m_render_layer = SERenderLayer::OTHERS;
	}
}
// Default Texture Ctor. Does not include 
//Texture::Texture(std::filesystem::path texture_path, bool _isAnimated) : isAnimated(isAnimated)
//{
//	std::unordered_map < std::string, std::filesystem::path> tmp;
//	tmp.emplace(texture_path.filename().string(), texture_path);
//
//	AnimationTextures.emplace(ACTION::ACTION_NULL, tmp);
//	
//	compType = COMPONENT_TYPE::CT_TEXTURE;
//	SE
// (texture_path);
//	TextureData = SEGetTexture(texture_path);
//}
//
//Texture::Texture(std::list<std::filesystem::path> paths, bool is_Animated)
//{
//
//}
//
//
//void Texture::SetUV(const std::vector<SEVec2>& _uv)
//{
//	uv = _uv;
//}
//
//const std::vector<SEVec2>& Texture::GetUV()
//{
//	return uv;
//}
//
//void Texture::SetAnimationFlag(SEBool flag)
//{
//	isAnimated = flag;
//}
//
//const SEBool& Texture::GetAnimationFlag()
//{
//	return isAnimated;
//}
//
//void Texture::SetTexture(const std::filesystem::path& texture_path)
//{
//	std::unordered_map<std::string, std::filesystem::path> tmp;
//	tmp.emplace(texture_path.filename().string(), texture_path);
//	AnimationTextures.emplace(ACTION::ACTION_NULL, tmp);
//
//	// TODO: Stress test logic.
//	SELoadTexture(texture_path);
//	TextureData = SEGetTexture(texture_path);
//}
//
//const SEUint& Texture::GetTextureData()
//{
//	return TextureData;
//}

Font::Font(const std::filesystem::path& font, const std::string& text) : fontPath(font), m_text(text)
{
	compType = COMPONENT_TYPE::CT_FONT;
	SELoadFont(fontPath);
	FontData = SEGetFont(fontPath.filename().string());
}

void Font::SetFont(const std::filesystem::path& _font)
{
	fontPath = _font;
	SELoadFont(fontPath);
	FontData = SEGetFont(fontPath.filename().string());
}

const SEFont& Font::GetFont()
{
	return FontData;
}

const std::filesystem::path Font::GetFontPath()
{
	return fontPath;
}

//Node_Buffer::Node_Buffer(std::string node_name)
//{
//	this->node_name = node_name; 
//}
//
//Node_Buffer::~Node_Buffer()
//{
//	for (Option* option : option_objects)
//	{
//		delete option; 
//	}
//	option_objects.clear();
//}

Dialogue::Dialogue()
{
	compType = COMPONENT_TYPE::CT_DIALOGUE;
}

DialogueParent::DialogueParent()
{
	compType = COMPONENT_TYPE::CT_DIALOGUE_PARENT;
}

DialogueParent::DialogueParent(std::string child_node_name)
{
	starting_node_name = child_node_name;
}

GameObject::GameObject(const std::string& objectName) : m_name(objectName) {}

GameObject::~GameObject()
{
	auto it = m_componentArray.begin();
	while (it != m_componentArray.end())
	{
		if (it->second)
			delete it->second;
		++it;
	}

	/*for (const auto& comp : m_componentArray)
	{
		if (comp.second)
		{
			delete comp.second;
			m_componentArray.erase(comp.first);
		}
	}
	m_componentArray.clear();*/

#if _DEBUG
	//std::cout << "Game Object: Deallocated " << sizeof(*this) << " bytes\n";
#else


#endif
}

Component* GameObject::getComponent(COMPONENT_TYPE compType)
{
	for (auto& search : m_componentArray)
	{
		if (search.first == compType)
			if (search.second)
				return search.second;
	}
	return 0;
}

void GameObject::addComponent(Component* pComp)
{
	if (pComp)
		m_componentArray.emplace(pComp->compType, pComp);
}

void GameObject::addComponent(COMPONENT_TYPE type)
{
	switch (type)
	{
	case COMPONENT_TYPE::CT_COLLISION:
	{
		std::unique_ptr<Collision> col = std::make_unique<Collision>();
		m_componentArray.emplace(CT_COLLISION, col.release());
		break;
	}
	case COMPONENT_TYPE::CT_COLOR:
	{
		std::unique_ptr<Color> color = std::make_unique<Color>();
		m_componentArray.emplace(CT_COLOR, color.release());
		break;
	}
	case COMPONENT_TYPE::CT_FONT:
	{
		std::unique_ptr<Font> font = std::make_unique<Font>();
		m_componentArray.emplace(CT_FONT, font.release());
		break;
	}
	case COMPONENT_TYPE::CT_TEXTURE:
	{
		std::unique_ptr<Texture> text = std::make_unique<Texture>();
		m_componentArray.emplace(CT_TEXTURE, text.release());
		break;
	}
	case COMPONENT_TYPE::CT_TRANSFORM:
	{
		std::unique_ptr<Transform> trans = std::make_unique<Transform>();
		m_componentArray.emplace(CT_TRANSFORM, trans.release());
		break;
	}
	case COMPONENT_TYPE::CT_DIALOGUE:
	{
		std::unique_ptr<Dialogue> dialg = std::make_unique<Dialogue>();
		m_componentArray.emplace(CT_DIALOGUE, dialg.release());
		break;
	}
	//case COMPONENT_TYPE::CT_DIALOGUE_HOLDER:
	//{
	//	std::unique_ptr<DialogueHolder> dialg_holder = std::make_unique<DialogueHolder>();
	//	m_componentArray.emplace(CT_DIALOGUE_HOLDER, dialg_holder.release());
	//	break;
	//}
	case COMPONENT_TYPE::CT_DIALOGUE_PARENT:
	{
		std::unique_ptr<DialogueParent> dialg_parent = std::make_unique<DialogueParent>();
		m_componentArray.emplace(CT_DIALOGUE_PARENT, dialg_parent.release());
		break;
	}
	default:
		break;
	}

}

void GameObject::deleteComponent(COMPONENT_TYPE type)
{
	for (auto& comp : m_componentArray)
	{
		if (comp.first == type)
		{
			if (comp.second)
				delete comp.second;
		}
	}
}

const std::string& GameObject::getName()
{
	return m_name;
}

const std::string& GameObject::getTag()
{
	return m_tag;
}

void GameObject::setTag(std::string tag)
{
	m_tag = tag;
}

//void SEAddAnimationToObject(std::string objectName, ACTION action, SEInt Frames, SEFloat speed)
//{
//	std::unordered_map<int, std::vector<SEVec2>> tmpContainer{};
//	GameObject* go = nullptr;
//	for (auto& obj : goArray)
//	{
//		if (obj.first == objectName)
//		{
//			if (obj.second.second)
//			{
//				go = obj.second.second;
//			}
//		}
//	}
//	if (go)
//	{
//		Texture* pTex = (Texture*)go->getComponent(COMPONENT_TYPE::CT_TEXTURE);
//		if (pTex)
//		{
//			pTex->SetAnimationFlag(1);			// logically, if goes into animating, flag should be 1 no matter what.
//			pTex->AnimationSpeed = speed;
//			//pTex->Frames = Frames;
//			for (int i = 0; i < Frames; ++i)
//			{
//				SEVec2 TL = { 0.0f + (float)i / Frames			, 1};
//				SEVec2 TR = { 0.0f + (float)(i + 1.0f) / Frames	, 1};
//				SEVec2 BL = { 0.0f + (float)i / Frames			, 0};
//				SEVec2 BR = { 0.0f + (float)(i + 1.0f) / Frames  ,0};
//				std::vector<SEVec2> uv;
//				uv.push_back(BL);
//				uv.push_back(BR);
//				uv.push_back(TR);
//				uv.push_back(TL);
//				
//				tmpContainer.emplace(i, uv);
//			}
//			pTex->AnimationUV.emplace(action, tmpContainer);
//		}
//	}
//}


std::unordered_map<int, std::vector<SEVec2>> Texture::SEComputeUV(const int& Frames)
{
	std::unordered_map<int, std::vector<SEVec2>> UVs;

	for (int i = 0; i < Frames; ++i)
	{
		SEVec2 TL = { 0.0f + (float)i / Frames			, 1 };
		SEVec2 TR = { 0.0f + (float)(i + 1.0f) / Frames	, 1 };
		SEVec2 BL = { 0.0f + (float)i / Frames			, 0 };
		SEVec2 BR = { 0.0f + (float)(i + 1.0f) / Frames  ,0 };
		std::vector<SEVec2> uv;
		uv.push_back(BL);
		uv.push_back(BR);
		uv.push_back(TR);
		uv.push_back(TL);

		UVs.emplace(i, uv);
	}
	return UVs;
}

std::unordered_map<int, std::vector<SEVec2>> Texture::SEComputeUV(const int& _Rows, const int& Cols, const int& Frames)
{
	std::unordered_map<int, std::vector<SEVec2>> uv_store;
	int count = 0;
	const float dx = 1.0f / Cols;
	const float dy = 1.0f / _Rows;
	for (int j = 0; j < _Rows; ++j)
	{
		for (int i = 0; i < Cols; ++i)
		{
			if (count >= (Frames)) break;

			SEVec2 TL = { i * dx, 1.0f - (j)*dy };
			SEVec2 TR = { (i + 1.0f) * dx, 1.0f - j * dy };
			SEVec2 BR = { (i + 1.0f) * dx,  1.0f - (j + 1.0f) * dy };
			SEVec2 BL = { i * dx, 1.0f - (j + 1.0f) * dy };
			std::vector<SEVec2> uv;
			uv.push_back(BL);
			uv.push_back(BR);
			uv.push_back(TR);
			uv.push_back(TL);
			uv_store.emplace(count, uv);

			count++;
		}
	}

	return uv_store;
}

std::string Texture::GetCurrentRenderLayer()
{
	std::string ret_layer = "";
	if (m_render_layer == SERenderLayer::BACKGROUND)
	{
		ret_layer = "Background";
	}
	else if (m_render_layer == SERenderLayer::GAMEPLAY)
	{
		ret_layer = "Gameplay";
	}

	else if (m_render_layer == SERenderLayer::FOREGROUND)
	{
		ret_layer = "Foreground";
	}

	else if (m_render_layer == SERenderLayer::UI_ELEMENTS)
	{
		ret_layer = "UI Elements";
	}

	else if (m_render_layer == SERenderLayer::OVERLAY)
	{
		ret_layer = "Overlay";
	}

	else
	{
		ret_layer = "UNRECOGNIZED LAYER! ";
	}

	return ret_layer;
}

void SEAddAnimationToObject(std::string objectName, std::filesystem::path texture, ACTION action, SEInt Frames, SEFloat speed)
{
	std::unordered_map<int, std::vector<SEVec2>> UVs{};
	std::unordered_map<std::string, std::filesystem::path> TexturesContainer{};

	GameObject* go = nullptr;
	for (auto& obj : goArray)
	{
		if (obj.first == objectName)
		{
			if (obj.second.second)
			{
				go = obj.second.second;			// Find gameobject by looking for name from goArray
			}
		}
	}
	if (go)
	{
		Texture* pTex = (Texture*)go->getComponent(COMPONENT_TYPE::CT_TEXTURE);
		if (pTex)
		{
			pTex->IsAnimating = 1;			// logically, if goes into animating, flag should be 1 no matter what.
			//pTex->AnimationSpeed = speed;
			//pTex->Frames = Frames;
			std::unordered_map<int, std::vector<SEVec2>> uvs = pTex->SEComputeUV(Frames);
			//for (int i = 0; i < Frames; ++i)
			//{
			//	SEVec2 TL = { 0.0f + (float)i / Frames			, 1 };
			//	SEVec2 TR = { 0.0f + (float)(i + 1.0f) / Frames	, 1 };
			//	SEVec2 BL = { 0.0f + (float)i / Frames			, 0 };
			//	SEVec2 BR = { 0.0f + (float)(i + 1.0f) / Frames  ,0 };
			//	std::vector<SEVec2> uv;
			//	uv.push_back(BL);
			//	uv.push_back(BR);
			//	uv.push_back(TR);
			//	uv.push_back(TL);
			//
			//	UVs.emplace(i, uv);
			//}
			SELoadTexture(texture);
			TexturesContainer.emplace(texture.filename().string(), texture);
			pTex->AnimationUV.emplace(action, uvs);
			pTex->AnimationTextures.emplace(action, TexturesContainer);
			pTex->AnimationFrames.emplace(action, Frames);
			pTex->AnimationSpeed.emplace(action, speed);
		}
	}
}

std::unordered_map<int, std::vector<SEVec2 >> SEGetAnimationUV(std::string objectName, ACTION action)
{
	std::vector<SEVec2> tmp{};
	GameObject* go = nullptr; (void)go;
	for (auto& obj : goArray)
	{
		//if (obj.first == objectName)
		if (obj.second.second->getName() == objectName)
		{
			if (obj.second.second)
			{
				// Check for the relevant action in the container, then return.
				Texture* pTex = (Texture*)obj.second.second->getComponent(CT_TEXTURE);
				if (pTex)
				{
					for (auto& texture : pTex->AnimationUV)
					{
						if (texture.first == action)
						{
							return texture.second;
						}
					}
				}
			}
		}
	}
	return std::unordered_map<int, std::vector<SEVec2 >>{};

	/*if (go)
	{
		Texture* pTex = (Texture*)go->getComponent(COMPONENT_TYPE::CT_TEXTURE);
		if (pTex)
		{
			auto search = pTex->AnimationUV.find(action);
			if (search != pTex->AnimationUV.end())
			{
				return search->second;
			}
		}
	}*/
}




void SEDeleteAllObject()
{

	/*for (auto& obj : goArray)
	{
		if (obj.second.second)
		{
			delete obj.second.second;
		}
		goArray.erase(obj.first);
	}*/

	// TODO: Mem leak !

	/*for (auto& obj : goArray)
	{
		GameObject* go = obj.second.second;
		if (go)
		{
			std::unordered_multimap<COMPONENT_TYPE, Component*> components = go->m_componentArray;
			components.clear();
		}
	}

	goArray.clear();*/

	for (const auto& obj : goArray)
	{
		if (obj.second.second)
		{
			if (!obj.second.second->getComponent(CT_DIALOGUE)) // do not delete <Dialogue Objects> keep it in the goArray.
			{
				delete obj.second.second;
			}
		}
	}

	goArray.clear();

}


void SEFactoryExit()
{
	/*for (auto& obj : gameObjectArray)
	{
		if (obj.second)
		{
			delete obj.second;
		}
	}*/
	for (auto& obj : goArray)
	{
		if (obj.second.second)
		{
			delete obj.second.second;
		}
	}
	//goArray.clear();
	SEWrite("Factory obj all killed\n");
}

void SESetObjectScale(std::string gameobject, float scale_x, float scale_y)
{
	auto go = SEFindObjectByName(gameobject);
	if (go)
	{
		Transform* t = (Transform*)go->getComponent(COMPONENT_TYPE::CT_TRANSFORM);
		if (t)
		{
			t->setScale({ scale_x, scale_y });
		}
	}
}

void SESetObjectPosition(std::string gameobject, float x, float y)
{
	auto go = SEFindObjectByName(gameobject);
	if (go)
	{
		Transform* t = (Transform*)go->getComponent(COMPONENT_TYPE::CT_TRANSFORM);
		if (t)
		{
			t->setTranslate({ x,y });
		}
	}
}

void SESetObjectRotation(std::string gameobject, float angle, float speed)
{
	auto go = SEFindObjectByName(gameobject);
	if (go)
	{
		Transform* t = (Transform*)go->getComponent(COMPONENT_TYPE::CT_TRANSFORM);
		if (t)
		{
			if (speed > 0)
				t->setRotateFlag(1);
			t->setRotationAngle(angle);
			t->setRotationSpeed(speed);
		}
	}
}

ObjectiveList::ObjectiveList(SEFont _font, std::filesystem::path ObjectiveListFile, SEVec2 pos, SEFloat scale, SEVec3 color)
{
	ObjectivesVEC = SEReadObjectiveList(ObjectiveListFile);

	m_font = _font;
	Lines = {};
	m_startingpos = m_LastMemberPos = pos;
	m_spacing = scale * 0.001f;
	m_font_scale = scale;
	m_color = color;

	Generate_Strikeout();
}

void ObjectiveList::Generate_Strikeout() {
	int c = 0;
	std::string l = {};
	for (auto Ostr : ObjectivesVEC) {
		for (int i = 0; i < Ostr.length()*0.6f; i++) {
			l += "_";
		}
		Lines.push_back(l);
		c++;
	}
}

void ObjectiveList::OL_DrawList()
{
	int i = 0;
	for (auto ObjStr : ObjectivesVEC) {
		SEDrawFont(ObjStr, m_font, m_LastMemberPos.x, m_LastMemberPos.y, m_font_scale, m_color.r, m_color.g, m_color.b);

		if (current_level == LEVEL::LEVEL_1)
		{
			if (SEGetGameProgress((ProgressFlags_LV1)i)) {
				SEDrawFont("_______________", m_font, m_LastMemberPos.x, m_LastMemberPos.y + 5, m_font_scale, m_color.r, m_color.g, m_color.b);
			}

			m_LastMemberPos.y = m_LastMemberPos.y - m_font_scale - m_spacing;
			i++;
		}

		if (current_level == LEVEL::LEVEL_2)
		{
			if (SEGetGameProgress((ProgressFlags_LV2)i)) {
				SEDrawFont("_______________", m_font, m_LastMemberPos.x, m_LastMemberPos.y + 5, m_font_scale, m_color.r, m_color.g, m_color.b);
			}

			m_LastMemberPos.y = m_LastMemberPos.y - m_font_scale - m_spacing;
			i++;
		}
	
	

	
	}

	
}

void ObjectiveList::ResetCounter(int& counter)
{
	counter = 0;
}
