/******************************************************************************//**
 * \file		SEScripting.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (100%)
 *
 * \brief		Scripting Logic 
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEGraphics.h"

#define DEFAULT_TIMER 4
#define MAX_MOVEMENT_SPEED 500
#define MIN_MOVEMENT_SPEED 250
#define NPC_MOVEMENT_SPEED 100.0f
#define MAX_BAR_SCALE 27.5

//scale fixing
#define HOLDBROOM_SCALE 1.05f
#define CLEANING_SCALE 1.2f
#define BROOM_SCALE 2
#define SEARCHING_SCALE 1.05f
#define SEARCH_VFX_SCALE 50

static SEVec2 VFXDefScale{};

// Forward declare to prevent cyclic referencing.
class GameObject;
class Component;
class Transform;
class Texture;
class Collision;
class Dialogue;
enum class ACTION;
enum class LEVEL;
enum class ProgressFlags_LV1;
enum class ProgressFlags_LV2;
enum class DialogueMuseCheck;
enum class DialogueMuseCheckLv2;
enum class DialogueLalaCheck;
enum class DialogueLalaCheckLv2;
enum class DialogueAkaiCheck;
enum class ChaseRecords_LV1;
enum class ChaseRecords_LV2; 

// Final 
enum class ChaseLalaRecords_LV1; // [Level 1 - Lala Checks] 
enum class ChaseLalaRecords_LV1;
enum class ChaseLalaRecords_LV2;
enum class ChaseMuseRecords_LV2; // Added 31/3
enum class ChaseAkaiRecords_LV2; // Added 31/3
enum class NPC_MUSE_STATE;
enum class NPCLalaRoute;
enum class NPCMooseRoute;

bool SEPlayerQuit();

namespace GAME_UI {
	static bool isPause;
}

SEVec2 Bounded_Cam(SEVec2 _translate, SEVec2 _Cam_dimensions, SEVec4 _WorldBound);

/**
	 * Get current state of progress in the level
 */
// Function Overloading
bool SEGetGameProgress(ProgressFlags_LV1 _progress); 
bool SEGetGameProgress(ProgressFlags_LV2 _progress);

/**
	 Reset all game progress to starting values
 */

void SEResetGameProgress();

/**
	 * Set current score to i
 */
void ToggleGameProgressLv1(ProgressFlags_LV1 _progress);
void ToggleGameProgressLv2(ProgressFlags_LV2 _progress);
void ToggleALLGameProgress();
//
//std::string GetButtonName(Behaviour_Button::Button_Selected selection);

bool SEGetDialogueMuseStatus(DialogueMuseCheck muse_dialogue);
bool SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2 muse_dialogue);
void ToggleDialogueMuseStatus(DialogueMuseCheck muse_dialogue);
void ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2 muse_dialogue);

bool SEGetDialogueLalaStatus(DialogueLalaCheck lala_dialogue);
bool SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2 lala_dialogue);
void ToggleDialogueLalaStatus(DialogueLalaCheck lala_dialogue);
void ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2 lala_dialogue);

bool SEGetDialogueAkaiStatus(DialogueAkaiCheck Akai_dialogue);
void ToggleDialogueAkaiStatus(DialogueAkaiCheck Akai_dialogue);

// Level 1 - Chase Records
// -----------------------------------------
bool SEGetChaseRecord(ChaseRecords_LV1 chase_status);				 // Muse [LV1]
bool SEGetChaseLalaRecord(ChaseLalaRecords_LV1 chase_lala_status);   // Lala [LV1]
void ToggleLalaChaseRecordLV1(ChaseLalaRecords_LV1 chase_lala_status);

// Added on 31/3/2023
// Level 2 - Chase Records
// -----------------------------------------
bool SEGetChaseRecord(ChaseMuseRecords_LV2 chase_status);
bool SEGetChaseRecord(ChaseAkaiRecords_LV2 chase_status);
bool SEGetChaseRecord(ChaseLalaRecords_LV2 chase_status);
void ToggleMuseChaseRecord(ChaseMuseRecords_LV2 chase_status);
void ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2 chase_status);
void ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2 chase_status);




// Legacy
void ToggleChaseRecord(ChaseRecords_LV1 chase_status);
void ToggleChaseRecord(ChaseRecords_LV2 chase_status);

void PoofVFX(GameObject* poofObj);

SEVec2 Bounded_Cam(SEVec2 _translate, SEVec2 _Cam_dimensions, SEVec4 _WorldBound);

void SplashScreen();

/**
	 * base behaviour class. to be inheretied by other classes that require logic.
	 * serialisation also have to include a behaviour member. see objects that have behaviour class in .json
 */
class Behaviour
{
public:

	Transform* m_Transform = nullptr;
	Collision* m_Collision = nullptr;
	Texture* m_Texture = nullptr;

	enum class ITEM_TYPE
	{
		TOOL = 0,
		EVIDENCE,
		NPC,
		PLAYER
	};
	ITEM_TYPE m_Type;
	std::string behaviour_name;

	GameObject* go;
	Behaviour(GameObject* _go) : go(_go) {}
	
	
	virtual void Init() { std::cout << "base behaviour init\n"; };
	virtual void Update() {};
	virtual ~Behaviour() = default;
};



/**
	 * Behvaiour class for player
 */
class Behaviour_Player : public Behaviour
{
public:
	enum class INTERACTABLE
	{
		NONE,
		PAPER_REAL,
		PAPER_FAKE,
		PHOTO,
		PHOTO_FROM_PILE,
		BROOM_PUTDOWN,
		BROOM_PICKUP,
		MOP_PICKUP,
		MOP_BLOODY, // after mopping blood
		MOP_PUTDOWN,
		GLASSPIECES,
		TRASHBAG,
		KNIFE_UNWASHED,
		KNIFE_WASHED,
		INACTIVE, // general

		// Sink Toggle
		SINK_OFF,
		SINK_ON,

		// Paint Bucket 
		BUCKET_FAKE_BLOOD_FULL, 
		BUCKET_EMPTY,

		// Candy Horn
		REAL_CANDY_HORN, 
		FAKE_CANDY_HORN, 
		CANDY_PILE_SEARCHED, 

		INTERACTABLE_ENUM_COUNT
	};

	int m_health;
	float m_movementSpeed;
	ACTION m_currentWalking, m_currentIdle, m_currentCleaning, m_currentSearching, m_CurrentAction;
	INTERACTABLE m_currentHoldingItem;
	SEVec2 m_CurrentPos{}, m_PreviousPos{};

	bool scale_init = true;
	float c_xscale = 0;
	bool movedDir[4];

public:

	Behaviour_Player(GameObject* _go) : Behaviour(_go) {}
	void Init() override;
	void Update() override;

};

/**
	 * Button scripting logic
 */
class Behaviour_Button : public Behaviour
{
public:

	Dialogue* m_Dialogue;
	ACTION m_CurrentAction;

	//LEVEL nextScene, prevScene;
	//std::string nextScene, prevScene;
	//std::string behaviour_name = "Button";

	//std::string nextScene;

	// Triggers suspicious event. 
	SEUint suspicion_value;
	SEBool is_clicked;
	std::string parent_npc;

	void SusRotateArrow();
	void CheckRenderOff();
	
	Behaviour_Button(GameObject* _go) : Behaviour(_go) {}

	void Init() override;
	void Update() override;

};

/**
	 * Button scripting logic
 */
class Behaviour_Arrow : public Behaviour {

public:

	ACTION m_currentAction;


	Behaviour_Arrow(GameObject* _go) : Behaviour(_go) {}

	void Init() override;
	void Update() override;

};

/**
	 * NPC scripting logic
 */
class Behaviour_NPC : public Behaviour
{
public:
	// For Scaling stuff (left or right)
	enum class NPCFacing : int
	{
		START, // for prev_facing (first time)
		LEFT,
		RIGHT, 
		UP, 
		DOWN
	};

	// For Detection Cone
	enum class NPCFacingY : int
	{
		UP, 
		DOWN,
	};

	SEVec2 currentWaypoint;
	int waypointIndex;
	int currentSequence;
	int m_prevMoveDir;
	int m_moveDir;
	float m_movementSpeed;
	ACTION m_currentAction;
	std::vector<std::vector<SEVec2>> Waypoints;								     // Muse's Waypoints
	std::vector<std::pair<NPCLalaRoute, std::vector<SEVec2>>> lala_waypoints;    // Lala's Waypoints
	std::vector<std::pair<NPCMooseRoute, std::vector<SEVec2>>> moose_waypoints;  // Moose's Waypoints

	SEInt current_waypoint_index = 0; // 3/16/2023
	SEVec2 direction;

	// For Cone Facing - 3/23/2023s
	NPCFacing buffer_prev_npc_facing;				// [Temp Fix?] There was an instance where npc will translate out of waypoint (because of movement per frame) then will try to adjust back
	NPCFacing prev_npc_facing = NPCFacing::START; 
	NPCFacing npc_facing;	   

	std::map<std::string, SEBool> Dialogue_Checker;
	SEBool toggle_off = false;

	// Chase Zone
	std::string chase_zone_name; // name of the gameObject that is considered the "chase zone" or parameter where the NPC can chase until before going back to his initial waypoint. 
	NPC_MUSE_STATE must_state;

	// Flags for Waypoint checks (x & y axis)
	bool x_wp_check = false;
	bool y_wp_check = false;

	// For calling (init) in reset.
	SEBool waypoint_init_once = false;

	Behaviour_NPC(GameObject* _go):Behaviour(_go) {}
	void Init() override;
	void Update() override;

	SEBool CollidePlayer();
	void MoveTowardsTarget(GameObject* go, float speed = 1.0);
	void MoveTowardsPosition(SEVec2 position, float speed = 1.0);

	void MoveToWaypoint(NPCLalaRoute lala_route, Transform* npc_transform, NPCFacing final_pos_npc_facing); // check if predicted position reaches the waypoint positions. 
	void MoveToWaypoint(NPCMooseRoute moose_route, Transform* npc_transform, SEBool looping = false);
	void MoveToWaypoint(NPCLalaRoute lala_route, Transform* npc_transform, SEBool looping = false);
	void UpdateNPCFacing();
};

/**
	 * Evidence scripting logic
 */
class Behaviour_Evidence : public Behaviour
{
public:

	ACTION m_currentAction;
	SEBool evidence_taken; 

	Behaviour_Evidence(GameObject* _go) :Behaviour(_go) {}
	void Init() override;
	void Update() override;
};

class Behaviour_GameplayUI : public Behaviour {
public: 

	SEVec2 current_CameraPos;
	SEVec2 WinVec;
	ACTION m_currenAction;
	bool m_UIstatus;
	SEVec2 Parent_Pos;
	float VFX_PLeft;
	float VFX_PRight;
	SEBool sus_change = false;

	Behaviour_GameplayUI(GameObject* _go) :Behaviour(_go) {}

	void Init() override;
	void Update() override;

	void SusGlow();
	void SusStopGlow();
	void SusArrowRotate();
	void TriggerLoseScreen();
	void TriggerWinScreen();
};

class Behaviour_Trigger_Meter : public Behaviour
{
public:

	SEUint suspicion_level;
	ACTION m_glow;

	Behaviour_Trigger_Meter(GameObject* _go) :Behaviour(_go) {}
		
	void SusRotateArrow();

	void Init() override;   // sets the starting values
	void Update() override; // glows + (TODO) move arrow (rotation)
};

// Acts as an invisble collider box for event trigger ("acts" as a trigger collision)
// ----------------------------------------------------------------------------------
// [Used In] ...
// [1] NPC Muse being alerted at the player when he enters his range when he sees player holding a piece of paper.
class Behaviour_Proximity : public Behaviour
{
public:
	Transform* m_Transform;  // Position / Scaling of the Trigger Box
	Collision* m_Collision;  // Collider Box.
	std::string npc_name;    // NPC tagged to this proximity.

	// For Alert Animation
	SEBool proximity_touched = false;
	SEBool alert_done = false;
	SEBool chasing = false;
	SEBool wasChasing = false;
	float make_shift_alert_timer = 0.f;
	float make_shift_esclamation_timer = 0.f;

	// 3/23/2023
	// Proximity Rotation (cone)
	float cone_rotated_value = 0; // will be reseted every loop

	// 3/27/2023
	// Chase Boolean 
	SEBool start_chase = false;

	Behaviour_Proximity(GameObject* _go) :Behaviour(_go) {}

	void Init() override;   // sets the starting 
	void Update() override; // glows + (TODO) move arrow (rotation)

	void TranslateVisionCone();
	void StopChasing();
};

class Behaviour_Esclamation : public Behaviour // for now named like that can be repurposed for other things.
{
public: 
	Transform* m_Transform;  // Position / Scaling of the Trigger Box
	Collision* m_Collision;  // Collider Box.
	ACTION m_currentAction;
	std::string npc_name;    // NPC tagged to this proximity.

	Behaviour_Esclamation(GameObject* _go) :Behaviour(_go) {}

	void Init() override;   // sets the starting 
	void Update() override; // glows + (TODO) move arrow (rotation)

};

class Behaviour_Splash : public Behaviour {
	
public:

	float m_DigiTimer, m_FMODTimer, m_IPSTimer;


	ACTION m_currentAction;

	Behaviour_Splash(GameObject* _go) :Behaviour(_go) {}

	void Init() override;
	void Update() override;
};

class Behaviour_Credits : public Behaviour {

public:


	ACTION m_currentAction;

	Behaviour_Credits(GameObject* _go) :Behaviour(_go) {}

	void Init() override;
	void Update() override;
};

void SEBehaviourUpdate(const LEVEL& _level);

struct Init_Level_Values {
	struct Entity
	{
		SEVec2 Pos{};
		int currentsequence{};

	}Player, Lala, Muse;

	struct InteractableEntity
	{
		Behaviour_Player::INTERACTABLE Occupant_Item{};
	}PaperReal, GlassPieces, Broom, PhotoFrame;

	struct GameplayLogicData
	{
		SEInt SusLevel{};
	}LogicData;
};