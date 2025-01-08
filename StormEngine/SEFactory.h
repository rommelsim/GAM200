/******************************************************************************//**
 * \file		SEFactory.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (60%)
 * \par    		Ming Lun (20%)
 * \par    		Ray (20%)
 *
 * \brief		Point of entry to create objects/prefabs in engine.
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SECollision.h"
#include "SEResource.h"
#include "SEScripting.h"


enum class LEVEL
{
	LEVEL_0 = 0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	MAIN_MENU,
	SETTINGS,
	EXIT,
	PAUSE,
	WIN,
	LOSE,
	HOW_TO_PLAY,
	CUTSCENE,
	ENDING,
	SPLASH,
	CREDITS,
};

//add as needed
LEVEL STRING_LEVEL(std::string _Level);

// [Specific Layer for render order]
// - in order from the furthest back to the furthest front. 
enum class SERenderLayer
{
	BACKGROUND,				// [0] - Background of the Game (Furnitures, Wallpapers, Windows)
	GAMEPLAY,				// [1] - Gameplay Elements	    (Interactables - Cops, Players (fox) , Evidence)
	FOREGROUND,				// [2] - Foreground				(Grassy Borders , House Frames)
	UI_ELEMENTS,			// [3] - UI Elements			(Suspicious Meter, Objective/Evidence left UI)
	OVERLAY, 				// [4] - Overlay				(Transition Screens, Text for brief story explanation, Digipen Splash Art)
	OTHERS
};

enum class SEDialogueType
{
	PROMPT,
	//PROMPT_LEFT,
	//PROMPT_RIGHT,
	OPTION, 
	BOARDER, 
	THOUGHT_BUBBLE, 
	NPC_NAME,			// 27/2/2023

	MISCELLEANEOUS		// 4/3/2023
};

class Animation
{
public:
	int i;
	float frameTimer;
	float interval;
public:
	Animation() : i(0), frameTimer(0.0f), interval(0.0f) {}

};

enum class ProgressFlags_LV1 : int {

	KEY_REPLACE_NOTE,
	KEY_DISPOSE_GLASS,
	KEY_REPLACE_PHOTO,
	DISPOSE_REAL_NOTE,
	SWEEP_GLASS,
	DISPOSE_REAL_PHOTO,
	

	PROGRESS_LV1_ENUM_COUNT
};

enum class ProgressFlags_LV2 : int
{
	// For Objective List (Main Objects) 
	REPLACE_BLOOD_WITH_PAINT,
	CLEAN_BLOOD_ON_KNIFE,
	REPLACE_HORN_WITH_CANDY, 
	
	// #Task 1 - Part (a)
	MOP_BLOOD_STAIN, 
	WASH_BLOOD_STAIN, 
	REPLACE_MOP,

	// #Task 1 - Part (b)
	TAKE_PAINT_BUCKET,
	POUR_FAKE_BLOOD, 
	PUT_BACK_EMPTY_BUCKET, 

	// #Task 2 
	THROW_AWAY_HORN, 
	CANDY_PILE_SEARCHED,
	PROGRESS_LV2_ENUM_COUNT

};

// Added 2/4/2023
enum class DialogueMuseCheck : int {
	NODE_1,
	NODE_9,
	NODE_10,
	NODE_11,
	NODE_12,
	


	DIALOGUE_MUSE_ENUM_COUNT
};

enum class DialogueMuseCheckLv2 : int {
	NODE_20,
	NODE_23,
	NODE_27,
	NODE_29,
	NODE_32,
	NODE_35,
	NODE_38,
	NODE_41,
	DIALOGUE_MUSE_ENUM_COUNT
};

enum class DialogueAkaiCheck : int {
	PROMPT_UNCHECKED, 
	NODE_21,
	NODE_24,
	NODE_26,
	NODE_30,
	NODE_33,
	NODE_36,
	NODE_39,
	NODE_42,
	DIALOUGE_AKAI_ENUM_COUNT
};

// Added 3/24/2023
enum class DialogueLalaCheckLv2 : int {
	NODE_22,
	NODE_25,
	NODE_28,
	NODE_31,
	NODE_34,
	NODE_37,
	NODE_40,
	NODE_43,
	DIALOGUE_LALA_ENUM_COUNT
};

enum class DialogueLalaCheck : int {

	// [Level 1 - Dialogues]
	NODE_2, 
	NODE_6, 
	NODE_7, 
	NODE_13, // Lala sees real photo
	NODE_14, // Lala sees unused broom
	NODE_15, // Lala sees thrash bag
	NODE_16, // lala sees fake photo
	PROMPT_UNCHECKED,

	// [Level 2 - Dialogues]


	DIALOUGE_LALA_ENUM_COUNT
};

// Added 3/4/2023
enum class ChaseRecords_LV1 : int {
	CHASE_REAL_PAPER,
	CHASE_FAKE_PAPER, 
	CHASE_BROOM,
	CHASE_BROOM_PUTDOWN,
	CHASE_RUBBISH_BAG, 
	CHASE_REAL_PHOTO, 
	CHASE_FAKE_PHOTO, 

	CHASE_ENUM_COUNT

};

// Lala Chase Records - Responsible for Chasing + Dialogue
enum class ChaseLalaRecords_LV1 : int {
	CHASE_BROOM, 
	CHASE_BROOM_PUTDOWN,
	CHASE_RUBBISH_BAG, 
	CHASE_REAL_PHOTO, 
	CHASE_FAKE_PHOTO, 

	CHASE_ENUM_COUNT
};

enum class ChaseLalaRecords_LV2 : int {

	// Level 2 -Task #1
	// -----------------------
	// Mop
	CHASE_CLEAN_MOP, 
	CHASE_BLOODY_MOP, 

	// Paint Bucket
	CHASE_PAINT_BUCKET, 
	CHASE_EMPTY_BUCKET, 


	// Level 2 -Task #2
	// -----------------------
	// Bloody Knife 
	CHASE_BLOODY_KNIFE, 
	CHASE_CLEAN_KNIFE, 

	// Level 2 -Task #3
	// -----------------------
	// Candy Horn
	CHASE_CANDY_HORN, 
	CHASE_CANDY_CORN, 

	CHASE_ENUM_COUNT
};



// Added 3/24/2023
enum class ChaseMuseRecords_LV2 : int {
	CHASE_CLEAN_MOP, 
	CHASE_BLOODY_MOP, 
	CHASE_FILLED_BUCKET, 
	CHASE_EMPTY_BUCKET, 
	CHASE_BLOODY_KNIFE, 
	CHASE_CLEAN_KNIFE, 
	CHASE_CANDY_HORN, 
	CHASE_CANDY_CORN, 

	CHASE_ENUM_COUNT
};

enum class ChaseAkaiRecords_LV2 : int {
	CHASE_CLEAN_MOP,
	CHASE_BLOODY_MOP,
	CHASE_FILLED_BUCKET,
	CHASE_EMPTY_BUCKET,
	CHASE_BLOODY_KNIFE,
	CHASE_CLEAN_KNIFE,
	CHASE_CANDY_HORN,
	CHASE_CANDY_CORN,

	CHASE_ENUM_COUNT
};

// Added 3/9/2023
enum class ButtonRoute : int {
	// [Restart Button] - Pause Menu
	RESTART_PROMPT,

	MAINMENU_PROMPT,

	EXIT_PROMPT, 

	RETRY_PROMPT,

	NONE


};

// Added 3/15/2023
enum class NPCLalaRoute : int {
	LV1_ROUTE_1, 
	LV2_ROUTE_1
};

// Added 3/18/2023
enum class NPCMooseRoute : int {
	LV1_GUARD,
	LV2_LINK,
	LV2_ROUTE_1,	// Patrol around the bush in lv2
};


enum class ACTION : int
{
	ACTION_NULL = 0,
	ACTION_IDLE,
	ACTION_WALKING,
	ACTION_CLEANING,
	ACTION_ALERT,
	ACTION_GLOW,
	ACTION_SEARCHING,
	ACTION_BUTTON_ISHOVERED,
	ACTION_BUTTON_IDLE,
	ACTION_CLEANING_BROOM,
	ACTION_IDLE_BROOM,
	ACTION_WALKING_BROOM,
	ACTION_IDLE_PAPER,
	ACTION_WALKING_PAPER,
	ACTION_IDLE_PHOTO,
	ACTION_WALKING_PHOTO,
	ACTION_IDLE_TRASHBAG,
	ACTION_WALKING_TRASHBAG,
	ACTION_ACTIVE_REAL,
	ACTION_ACTIVE_FAKE,
	ACTION_ACTIVE_GLASS,
	ACTION_ACTIVE_TRASHBAG,
	ACTION_OBJECTIVE_OPEN,
	ACTION_OBJECTIVE_CLOSE,
	ACTION_OPEN_HOW_TO_PLAY,
	ACTION_CLOSE_HOW_TO_PLAY,
	ACTION_LOSE_SCREEN,

	ACTION_ACTIVE,
	ACTION_INACTIVE,

	ACTION_EXCLAMATION,     // Exclamation Animation
	ACTION_POOFING,			// Pick up poof Animation

	// Level 2 - Animations
	ACTION_IDLE_MOP,
	ACTION_WALKING_MOP,
	ACTION_CLEANING_MOP,
	ACTION_IDLE_BLOODY_MOP,
	ACTION_WALKING_BLOODY_MOP,
	ACTION_WASHING,
	ACTION_BUCKET_FULL,					// [Paint Bucket] -> state when bucket still has the paint. 
	ACTION_BUCKET_EMPTY,				// [Paint Bucket] -> state of the bucket when player returns bucket after use. 
	ACTION_IDLE_FULL_PAINT_BUCKET,	    // [Player]       -> Idle animation with paint bucket (filled)
	ACTION_WALKING_FULL_PAINT_BUCKET,   // [Player]       -> Walking animation with paint bucket (filled)
	ACTION_FAKE_BLOOD,				    // [Pink Blood]   -> Texture for Fake Blood (without candy)
	ACTION_IDLE_EMPTY_PAINT_BUCKET,
	ACTION_WALKING_EMPTY_PAINT_BUCKET,
	ACTION_POURING_PAINT,				// [Player w Paint Bucket] -> Pouring paint bucket when -> placeholder for fake blood is empty. (texture)

	ACTION_IDLE_BLOODY_KNIFE,
	ACTION_WALKING_BLOODY_KNIFE,
	ACTION_IDLE_CLEAN_KNIFE,
	ACTION_WALKING_CLEAN_KNIFE,
	ACTION_CLEANED_KNIFE,				// Clean Knife on Table

	ACTION_REAL_HORN,
	ACTION_FAKE_HORN, 
	ACTION_IDLE_CANDY_HORN, 
	ACTION_WALKING_CANDY_HORN, 
	ACTION_IDLE_FAKE_CANDY_CORN, 
	ACTION_WALKING_FAKE_CANDY_CORN, 
	ACTION_SEARCH_FAKE_HORN,

	ACTION_JACK_IN_BOX_WINDING,

	ACTION_ENUM_COUNT		//DO NOT USE OTHER THAN DETERMINING THE SIZE (NO. OF ELEMENTS) OF THIS ENUM CLASS
};

std::string ACTION_STRING(ACTION eAction);



enum COMPONENT_TYPE
{
	CT_NULL = 0,
	CT_COLOR,			// done
	CT_TRANSFORM,		// done
	CT_COLLISION,		// done
	CT_TEXTURE,			// done
	CT_FONT,			// done
	CT_EVIDENCE,
	CT_TOOLS, 
	CT_DIALOGUE,		// Added [1/29/2023] to store the dialogue text (prompt / options)
	CT_DIALOGUE_PARENT  // Added [2/22/2023] to keep track which "GameObjects" or "NPCs" hold [Dialogue] Objects.
};


class Component
{
public:
	COMPONENT_TYPE compType;
	virtual ~Component();
};

class Color : public Component
{
public:
	Color();
	~Color() = default;
	void setColor(const SEVec3& c);
	const SEVec3& getColor();

private:
	SEVec3 m_color;
};

class Transform : public Component
{
public:
	Transform();
	~Transform() = default;

	// setters
	void setScale(const SEFloat& multiplier);
	void setScale(const SEFloat& multiplier_x, const SEFloat& multiplier_y);
	void setScale(const SEVec2& scale);
	void setRotationAngle(const SEFloat& angle);
	void setRotationSpeed(const SEFloat& speed);
	void setTranslate(const SEVec2& trans);
	void setModelTransform(const SEMat3& mdl_transform);
	void setModelToNdcTransform(const SEMat3& mdl_to_ndc_transform);
	void setAABBToNdcTransform(const SEMat3& AABB_to_ndc_transform);
	void setRotateFlag(const SEBool& flag);

	// gettters
	const SEVec2&	getScale();
	const SEFloat&	getRotationSpeed();
	const SEFloat&	getRotationAngle();
	const SEVec2&	getTranslate();
	const SEMat3&	getModelTransform();
	const SEMat3&	getModelToNdcTransform();
	const SEMat3&	getAABBToNdcTransform();
	const SEBool&	getRotateFlag();

	// Added 3/8/2023
	SEVec2 m_starting_translate;
	SEVec2 m_starting_scale;

private:
	SEVec2	m_scale, m_rotate, m_translate;			
	SEFloat	m_rotate_angle = 0, m_rotate_speed;
	SEMat3	m_model_transform;
	SEMat3	m_model_to_ndc_transform;
	SEMat3	m_AABB_to_ndc_transform;
	SEBool	m_rotate_flag;


};

class Collision : public Component
{
public:
	Collision();
	~Collision();

	//void UpdateCollision();
	// "this" instance to point
	//bool CollisionCheck(glm::vec2 pos2);
	// "this" instance to BoundBox
	//bool CollisionCheck(Collision* pos_other);

	// BoundBox to BoundBox
	//bool CollisionCheck(Collision* pos1, Collision* pos2);
	// BoundBox to Point
	//bool CollisionCheck(Collision* pos1, glm::vec2 pos2);
	StormEngine::CollisionDir CollisionDirCheck(Collision* pos_other);

	//void UpdateCollision();
	void SetCollision(glm::vec2 pos, glm::vec2 size);
	void SetCollisionSize(glm::vec2 size);

	StormEngine::COLLISION_TYPE collisionType;
	StormEngine::AABB boundaries;
	StormEngine::AABBPos collisionPos;

	SEBool isActive;
	
	//Mesh debugMesh;
};

class Texture : public Component
{
public:
	//std::vector<SEVec2> uv;
	//std::filesystem::path texture;
	//SEBool isAnimated;
	//SEBool isRendering;
	//SEUint TextureData;
	
public:
	Texture();
	Texture(std::filesystem::path texture_path, bool _isAnimated, SERenderLayer render_layer, int render_priority);		// Sets our render layer & loads texture data into resource manager.
	std::unordered_map<int, std::vector<SEVec2>> SEComputeUV(const int& Frames);
	std::unordered_map<int, std::vector<SEVec2>> SEComputeUV(const int& Rows, const int& Cols, const int& Frames);

	std::string GetCurrentRenderLayer();

	std::unordered_map < ACTION, std::unordered_map<int,std::vector<SEVec2 >>> AnimationUV;
	std::unordered_map < ACTION, std::unordered_map<std::string, std::filesystem::path>> AnimationTextures;
	std::unordered_map < ACTION, bool> AnimationFlags;
	
	std::unordered_map < ACTION, int> Rows;
	std::unordered_map < ACTION, int> Columns;
	std::unordered_map < ACTION, int> AnimationFrames;
	std::unordered_map < ACTION, float> AnimationSpeed;

	Animation AnimationHandler;
	bool IsAnimating;
	bool KeepAspectRatio;

public:
	// Render Layers 
	std::filesystem::path m_texture_path;		// Texture Path
	SEUint m_texture_data;						// Texture Data (in resource manager)
	SEBool isRendering = true;					// Rendering or not?
	SEInt  m_priority_level;					// GameObject's render priority. (lower the number, gets rendered first) [scale of 1 to 10]
	SERenderLayer m_render_layer;				// GameObject's render layer.

	// Functions
	void SEAssignJSONtoRenderLayer(std::string layer_string);		// [Helper Function] - helps to set "m_render_layer" based on the value given in JSON de-serialization

	std::filesystem::path isNotAnimating_Texture;

	// For retrieving still textures, for example background, buttons etc.
	const std::filesystem::path& GetTexture_NotAnimating() { return isNotAnimating_Texture; }
	
	// If want to retrieve animation textures, use SEPlayAnimation() instead.

public: 
	// save 2 button states textures.
	std::unordered_map <ACTION,std::string> dialogue_options_textures;
	SEBool dialogueboarder;
	

};

class Font : public Component
{
public:
	SEFont FontData;
	std::filesystem::path fontPath;
	std::string font;
	std::string m_text;

	// Added 2/27/2023 - Support Multiple Fonts + Colors
public:
	Font() = default;
	Font(const std::filesystem::path& font, const std::string& text = "");
	void SetFont(const std::filesystem::path& _font);
	const SEFont& GetFont();
	const std::filesystem::path GetFontPath();
public:
	// Dialogue Related Stuff
	SEVec2 font_scale;
	SEVec2 font_translate;

public:

};

class ObjectiveList {
public:
	SEVec2 m_startingpos;
	SEVec2 m_LastMemberPos;
	float m_spacing;
	float m_font_scale;
	SEFont m_font;
	SEVec3 m_color;
	std::vector<std::string> ObjectivesVEC;
	std::vector<std::string> Lines;
	SEVec2 m_OBpos;

	// Level Switching (for counter)
	LEVEL prev_level;
	LEVEL curr_level;

	ObjectiveList() = delete;
	ObjectiveList(SEFont _font, std::filesystem::path ObjectiveListFile, SEVec2 pos, SEFloat scale, SEVec3 color);

	void Generate_Strikeout();
	void OL_DrawList();
	void ResetCounter(int& counter);

};

// Added on [29/1/2023] 
// [Note] : this component will go hand in hand with the <Font> component. 
//			- since to even write anything, you would require a designated font 
// [Primary Function] - just to store the text stuff of a [SEDialougeNode] 
// [Acutal Nodes]	  - are still in <SEDialougeTree> -> dialogue_datbase.
class Dialogue : public Component
{
public:
	Dialogue();

	SEDialogueType dialogue_type;				    // [0] Prompt, [1] Option, [2] Boarder

	std::string dialogue_text;					   // Generic Placeholder for text.
	std::string parent_node_name;				   // Name of the node that this dialogue element belong to (prompt/option)
	std::vector<std::string> parent_node_names;	   // Added : 2/27/2023 to support elements that can be reused.

	// [1] Option related elements.
	SEInt suspicion_value;						   // [a] Value to increase (affects the sus metre)
	std::string next_node_name;					   // [b] Name of the next node this option will link to.

	std::string parent_npc;						   // Name of the NPC that holds this. 

	//std::vector<std::string> parent_npcs;		   // node belongs to many parents 

};

// Added on [22/2/2023] 
// GameObjects that "owns" the [Dialogue Object] 
class DialogueParent : public Component
{
public: 
	DialogueParent();
	DialogueParent(std::string child_node_name);

public: 
	std::string starting_node_name;					// A [GameObject] reference to the node that this object is holding. 
	std::vector<std::string> starting_node_names;   // 2/24/2023 - to support multiple dialogues per person.
};

//class Option 
//{
//public:
//	std::string next_node_name;
//	SEUint suspicion_level;
//	GameObject* option_object;
//public:
//	Option(std::string next_node, SEUint sus_level) : next_node_name(next_node_name), suspicion_level(sus_level) {}
//};
//
//class Node_Buffer
//{
//public:
//	GameObject* prompt_object;					// Stores reference to the actual [Prompt] Object.
//	std::vector<Option*> option_objects;        // Stores reference to the actual [Option] Objects.
//	std::string node_name; 
//public: 
//	Node_Buffer(std::string node_name);
//	~Node_Buffer();
//};
//
//
//class DialogueHolder : public Component
//{
//public:
//	DialogueHolder();
//	~DialogueHolder();
//public: 
//	std::vector<Node_Buffer*> node_database;
//public: 
//	Node_Buffer* SEFindNode(std::string node_name);
//};

class GameObject
{
private:
	static size_t count;
public:
	std::unique_ptr<Behaviour> m_behaviour;
	std::string m_BehaviourName{};
	GameObject(const std::string& objectName);
	GameObject(const GameObject& rhs, const std::string& name);
	~GameObject();
	GameObject* operator=(const GameObject* go);
	Component* getComponent(COMPONENT_TYPE compType);
	void	addComponent(Component* pComp);
	void	addComponent(COMPONENT_TYPE type);
	void deleteComponent(COMPONENT_TYPE type);
	const std::string&	getName();
	const std::string&	getTag();
	void setTag(std::string tag);

public:
	LEVEL m_level;
	std::string m_name;
	std::string m_tag;		// 1/17/2023 - used to identify whether it [GUI] or [Normal GameObject]
	std::unordered_multimap<COMPONENT_TYPE, Component*> m_componentArray;		// faster performance compared to ordered
	SEBool isActive;
	SEBool priority_swapped = false;
	//std::unordered_map<COMPONENT_TYPE, Component*> m_componentArray;		// faster performance compared to ordered
};


bool SECheckCollision(Collision* pos1, Collision* pos2);	// col to col
bool SECheckCollision(Collision* pos1, glm::vec2 pos2);		// col to vec

void SEAddAnimationToObject(std::string objectName, ACTION action, SEInt Frames, SEFloat speed);
void SEAddAnimationToObject(std::string objectName, std::filesystem::path texture, ACTION action, SEInt Frames, SEFloat speed);
void SEAddAnimationToObject(std::string objectName, std::filesystem::path texture, ACTION action, SEInt Frames, SEFloat speed);
std::unordered_map<int, std::vector<SEVec2 >> SEGetAnimationUV(std::string objectName, ACTION action);

void SEDeleteAllObject(); // kill all objects in the current array
void SEFactoryExit();

void SESetObjectScale(std::string gameobject, float scale_x, float scale_y);
void SESetObjectPosition(std::string gameobject, float x, float y);
void SESetObjectRotation(std::string gameobject, float angle, float speed);



//inline std::unordered_multimap<LEVEL, std::pair<std::string, GameObject*>> goArray;
inline std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>> goArray;

/**
 * Create an game object in the specifed level.
 * 
 * @param lvl Level to have the game object be in.
 * @param name Name of the game object.
 * @param components List of components for a game object.
 * 
 */
template <LEVEL lvl>
inline void SECreateObject(std::string name, std::list<Component*> components)
{
	std::unique_ptr<GameObject> go = std::make_unique<GameObject>(name);
	for (auto& comp : components)
	{
		go->addComponent(comp);
	}
	goArray.emplace(name, std::make_pair(lvl, go.release()));
}

// SECreateObject<LEVEL::LEVEL_0>("Sink", COMPONENT_TYPE::CT_TEXTURE, COMPONENT_TYPE::CT_TRANSFORM);
template <LEVEL lvl>
inline void SECreateObject(std::string name, int component_size, ...)
{

	//CT_COLOR = 0,		
	//CT_TRANSFORM,		
	//CT_COLLISION,		
	//CT_TEXTURE,	
	//CT_FONT,			
	//CT_EVIDENCE,
	//CT_TOOLS,
	//CT_NULL

	std::unique_ptr<GameObject> go = std::make_unique<GameObject>(name);
	COMPONENT_TYPE val;
	va_list vl;
	va_start(vl, component_size);
	for (int i = 0; i < component_size; ++i)
	{
		val = va_arg(vl, COMPONENT_TYPE);
		switch (val)
		{
		case COMPONENT_TYPE::CT_COLOR:
		{
			std::unique_ptr<Color> pColor = std::make_unique<Color>();
			go->addComponent(pColor.release());
			break;
		}
		case COMPONENT_TYPE::CT_TRANSFORM:
		{
			std::unique_ptr<Transform> ptrans = std::make_unique<Transform>();
			go->addComponent(ptrans.release());
			break;
		}
		case COMPONENT_TYPE::CT_COLLISION:
		{
			std::unique_ptr<Collision> pCol = std::make_unique<Collision>();
			go->addComponent(pCol.release());
			break;
		}
		case COMPONENT_TYPE::CT_TEXTURE:
		{
			std::unique_ptr<Texture> ptex = std::make_unique<Texture>();
			go->addComponent(ptex.release());
			break;
		}
		case COMPONENT_TYPE::CT_FONT:
		{
			std::unique_ptr<Font> pfont = std::make_unique<Font>();
			go->addComponent(pfont.release());
			break;
		}	
		case COMPONENT_TYPE::CT_DIALOGUE:
		{
			std::unique_ptr<Dialogue> pdialogue = std::make_unique<Dialogue>();
			go->addComponent(pdialogue.release());
			break;
		}
		case COMPONENT_TYPE::CT_DIALOGUE_HOLDER:
		{
			std::unique_ptr<DialogueParent> pdialogue_parent = std::make_unique<DialogueParent>();
			go->addComponent(pdialogue_parent.release());
			break;
		}
		default:
			break;
		}
	}
	goArray.emplace(name, std::make_pair(lvl, go.release()));
}

//template <LEVEL lvl>
//inline void SECreateObject(const GameObject* go)
//{
//	std::string name = go->m_name
//	goArray.emplace(name, std::make_pair(lvl, go.release()));
//}

inline void SECreateObject(LEVEL level, GameObject* go)
{
	std::string name = go->getName();
	goArray.emplace(name, std::make_pair(level, go));
}

/**
 * Overloaded function to create game object without specifying components. Will be preset with default comps.
 * 
 * \param name game object name
 */
//template <typename lvl>
//inline void SECreateObject(LEVEL lvl, std::string name, SERenderLayer layer, std::filesystem::path texture_path = "./Assets/Textures/Character_MainCharacter_Sprite.png", bool isAnimated = 0, float pos_x = 0.0f, float pos_y = 0.0f)
//{
//	std::unique_ptr<GameObject> go = std::make_unique<GameObject>(name);
//	std::unique_ptr<Transform> pTrans = std::make_unique<Transform>();
//	pTrans->setTranslate({ pos_x, pos_y });
//
//	std::unique_ptr<Texture> pTexture = std::make_unique<Texture>();
//	pTexture->m_texture_path = texture_path;
//	pTexture->m_render_layer = layer;
//	SELoadTexture(pTexture->m_texture_path);
//
//	std::unique_ptr<Collision> pColls = std::make_unique<Collision>();
//	pColls->SetCollision({ pos_x,pos_y }, { 50,50 });
//
//	go->addComponent(pTrans.release());
//	go->addComponent(pTexture.release());
//	go->addComponent(pColls.release());
//	goArray.emplace(name, std::make_pair(lvl, go.release()));
//
//}
//template <LEVEL lvl>
inline void SECreateObject(LEVEL lvl, std::string name, SERenderLayer layer, std::unique_ptr<Texture>& texComp, float pos_x = 0.0f, float pos_y = 0.0f)
{
	(void)layer;
	std::unique_ptr<GameObject> go = std::make_unique<GameObject>(name);
	std::unique_ptr<Transform> pTrans = std::make_unique<Transform>();
	pTrans->setTranslate({ pos_x, pos_y });

	std::unique_ptr<Collision> pColls = std::make_unique<Collision>();
	pColls->SetCollision({ pos_x,pos_y }, { 50,50 });
	
	go->addComponent(pTrans.release());
	go->addComponent(texComp.release());
	go->addComponent(pColls.release());
	go->m_level = lvl;
	goArray.emplace(name, std::make_pair(lvl, go.release()));
}

/**
 * Find a game object by it's name.
 * 
 * \param name name of game object to find
 * \return Game Object, null if not exist
 */
inline GameObject* SEFindObjectByName(std::string name)
{
	for (const auto& obj : goArray)
	{
		if (obj.second.second)
		{
			if (obj.second.second->getName() == name)
				return obj.second.second;
		}
	}
	return 0;
	/*auto it = goArray.find(name);
	if (it != goArray.end())
	{
		if(it->second.second)
			return it->second.second;
	}

	return nullptr;*/
}

/**
 * Returns a containter(unordered map) of game objects in the specified level.
 * 
 * \param lvl the game objects residing in that level
 * \return container of game objects
 */
inline const std::unordered_multimap<std::string, GameObject*> SEFindObjectsByLevel(LEVEL lvl)
{
	std::unordered_multimap<std::string, GameObject*> tmp;
	
	for (auto& obj : goArray)
	{
		if (obj.second.first == lvl)
		{
			tmp.emplace(obj.first, obj.second.second);
		}
	}
	return tmp;
}

/**
 * Returns the whole game's level.
 * 
 * \return container of all levels, which includes a pair of goName and go.
 */
inline std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>> SEGetAllLevels()
{
	return goArray;
}

template <LEVEL LevelType>
inline void SEDeleteObjectByName(std::string name)
{
	// added on 1/4/2023
	auto it = goArray.begin();
	while (it != goArray.end())
	{
		if (it->second.second->getName() == name)
		{
			delete it->second.second;		// delete gameObject*
			it = goArray.erase(it);			// delete map entry
		}
		else
			++it;
	}

	/*auto it = goArray.find(name);
	if (it != goArray.end())
	{
		if (it->second.second)
		{
			delete it->second.second;
		}
		goArray.erase(it);
	}*/
}

void inline SEDeleteComponent(GameObject* go, COMPONENT_TYPE comp_type)
{
	if (go)
	{
		
		
		auto search = go->m_componentArray.find(comp_type);
		if (search != go->m_componentArray.end())
		{
			if (search->second)
				delete search->second;
			go->m_componentArray.erase(search);
		}

	}
}