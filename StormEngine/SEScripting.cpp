/******************************************************************************/ /**
  * \file			SEScripting.cpp
  * \author 		Rommel (15%)
  * \co-author		Khair  (25%)
  * \co-author		Lee Yi Cong (25%)
  * \co-author		Ming Lun (35%)
  *
  * \contributions	zhenpengrommel.sim@digipen.edu
  *
  * \brief			C++ Scripting
  *
  * \date   		January 2023

 Copyright (C) 2023 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the
 prior written consent of DigiPen Institute of Technology is prohibited.
 ******************************************************************************/
#pragma once
#include "pch.h"
#include "SEScripting.h"
#include "SEFactory.h"
#include <SEAudio.h>
#include "SceneManager.h"
#include "SEDialogue.h"
#include <Timer.h>
#include "SEWaypointSystem.h"
#include "SEResource.h"

 /*Variables for gameplay logic*/
static const int MAX_progress_lv1_count = static_cast<int>(ProgressFlags_LV1::PROGRESS_LV1_ENUM_COUNT);

// placeholder
static const int MAX_progress_lv2_count = static_cast<int>(ProgressFlags_LV2::PROGRESS_LV2_ENUM_COUNT);

static SETimer ScriptTimer;
static SETimer dpTimer;
static SETimer fmTimer;
static const int MAX_dialogue_muse_count = static_cast<int>(DialogueMuseCheck::DIALOGUE_MUSE_ENUM_COUNT);
static const int MAX_dialogue_lala_count = static_cast<int>(DialogueLalaCheck::DIALOUGE_LALA_ENUM_COUNT);
static const int MAX_dialogue_akai_count = static_cast<int>(DialogueAkaiCheck::DIALOUGE_AKAI_ENUM_COUNT);

// Level 1 - Chase Count
static const int MAX_chase_count_lv1 = static_cast<int>(ChaseRecords_LV1::CHASE_ENUM_COUNT);
static const int MAX_Lala_Chase_count_lv1 = static_cast<int>(ChaseLalaRecords_LV1::CHASE_ENUM_COUNT);

// Level 2 - Chase Count
static const int MAX_Lala_Chase_count_lv2 = static_cast<int>(ChaseLalaRecords_LV2::CHASE_ENUM_COUNT);
static const int MAX_Muse_Chase_count_lv2 = static_cast<int>(ChaseMuseRecords_LV2::CHASE_ENUM_COUNT);
static const int MAX_Akai_Chase_count_lv2 = static_cast<int>(ChaseAkaiRecords_LV2::CHASE_ENUM_COUNT);

static int splashqueue = 0;
static int creditsqueue = 2;
static float holdtimer = 0.0f;
static bool SprintMode = false;
//static bool valuechange = false;

static float currSusMLevel = 0.f, prevSusMLevel = 0.f; // 0 for no suspicion, 1 for max suspicion (NORMALISED)
static bool Sus_Increase = false;
static bool objective_open = false;
static bool block_move = false;
static bool shard_prompt = false;
static bool photo_prompt = false;


// Dialogue Boolean
static bool touch_once_fake_paper = false;

// Lala flipping boolean
static bool stopFlipping = true;
static float lala_movement_speed = 3.0f;
static bool lala_stops_banging_her_head_back_and_forth = false;
static bool check_flip = false;
static bool check_move = true;

// Level 1 stuff
static bool GameProgressLv1[MAX_progress_lv1_count] = { false };
static bool DialogueMuseProgressLv1[MAX_dialogue_muse_count] = { false };
static bool DialogueLalaProgressLv1[MAX_dialogue_lala_count] = { false };
static bool ChaseStatusLv1[MAX_chase_count_lv1] = { false }; // Muse
static bool LalaChaseStatusLV1[MAX_Lala_Chase_count_lv1] = { false }; // Lala

// Level 2 stuff
static bool GameProgressLv2[MAX_progress_lv2_count] = { false };
static bool DialogueMuseProgressLv2[MAX_dialogue_muse_count] = { false };
static bool DialogueLalaProgressLv2[MAX_dialogue_lala_count] = { false };
static bool DialogueAkaiProgress[MAX_dialogue_akai_count] = { false };
//static bool ChaseStatusLv=2[MAX_chase_count] = { false };
//static bool ChaseStatusLv2[MAX_chase_count_lv2] = { false };
static bool MuseChaseStatusLv2[MAX_Muse_Chase_count_lv2] = { false };
static bool AkaiChaseStatusLv2[MAX_Akai_Chase_count_lv2] = { false };
static bool LalaChaseStatusLv2[MAX_Lala_Chase_count_lv2] = { false };

//static bool GameProgressLv12[]
static bool move_link = false;

//std::vector<LEVEL, SEBool> GameProgressLv1; 
//std::vector<LEVEL, SEBool> DialogueMuse;
//std::vector<LEVEL, SEBool> DialogueLalaProgressLv1;
//std::vector<LEVEL, SEBool> ChaseStatusLv1;

static SEVec2 debug;

// Current Level
extern LEVEL current_level = LEVEL::LEVEL_1;

// Button Routing Boolean
static ButtonRoute button_route_state;

static Behaviour_Player::INTERACTABLE PR1_currentOccuItem{};
static Behaviour_Player::INTERACTABLE PF25_currentOccuItem{};
static Behaviour_Player::INTERACTABLE GP26_currentOccuItem{};
static Behaviour_Player::INTERACTABLE B9_currentOccuItem{};
static Behaviour_Player::INTERACTABLE S21_currentOccuItem{};
static Behaviour_Player::INTERACTABLE M0_currentOccuItem{};
static Behaviour_Player::INTERACTABLE PB_currentOccuItem{}; // Paint Bucket
static Behaviour_Player::INTERACTABLE BK_currentOccuItem{}; // Blood Knife
static Behaviour_Player::INTERACTABLE CH_currentOccuItem{}; // Candy Horn
static Behaviour_Player::INTERACTABLE CP_currentOccuItem{}; // Candy Pile 

static bool playerquit{};

static SEVec2 PDef_Scale{};
static SEVec4 WorldBB{};
static float CameraDefaultHeight{};

Init_Level_Values InitVal;

bool SEPlayerQuit()
{
	return playerquit;
}

void RESET_LEVEL(LEVEL _Level)
{
	(void)_Level;
	// Reset Level
	current_level = LEVEL::LEVEL_1;
	// Reset Timer
	ScriptTimer.Reset();

	// Reset Suspicion Level
	currSusMLevel = 0;

	// Reset Cutscene
	cutscene_idx = 1;
	ending_idx = 1;

	// Linkage Waypoint for Akai
	move_link = false;

	// Reset NPC Waypont
	/*GameObject* npc_muse = SEFindObjectByName("NPC Muse32");
	Behaviour_NPC* npc_behave = dynamic_cast<Behaviour_NPC*>(npc_muse->m_behaviour.get());

	npc_behave->Init();*/

	// Reset Game Progress
	for (bool& progress_bool : GameProgressLv1)
	{
		progress_bool = false;
	}

	for (bool& progress_bool : GameProgressLv2)
	{
		progress_bool = false;
	}

	//if (SEGetGameProgress(ProgressFlags_LV1::)

	// Reset Dialouge Progress 
	for (bool& muse_bool : DialogueMuseProgressLv1)
	{
		muse_bool = false;
	}

	for (bool& lala_bool : DialogueLalaProgressLv1)
	{
		lala_bool = false;
	}

	for (bool& lala_bool : DialogueLalaProgressLv2)
	{
		lala_bool = false;
	}

	for (bool& lala_bool : DialogueMuseProgressLv2)
	{
		lala_bool = false;
	}

	for (bool& lala_bool : DialogueAkaiProgress) // level 2
	{
		lala_bool = false;
	}

	// Reset Chase Progress
	for (bool& chase_bool : ChaseStatusLv1)
	{
		chase_bool = false;
	}

	for (bool& chase_bool : LalaChaseStatusLv2)
	{
		chase_bool = false;
	}

	for (bool& chase_bool : MuseChaseStatusLv2)
	{
		chase_bool = false;
	}

	for (bool& chase_bool : AkaiChaseStatusLv2)
	{
		chase_bool = false;
	}


	// Reset all [Entity] positions. 
	for (auto go_pair : goArray)
	{
		GameObject* go = go_pair.second.second;
		Behaviour_Evidence* evidence_behave = dynamic_cast<Behaviour_Evidence*>(go->m_behaviour.get());
		Behaviour_NPC* npc_behave = dynamic_cast<Behaviour_NPC*>(go->m_behaviour.get());
		Behaviour_Proximity* proximity_behave = dynamic_cast<Behaviour_Proximity*>(go->m_behaviour.get());
		Behaviour_GameplayUI* ui_behave = dynamic_cast<Behaviour_GameplayUI*>(go->m_behaviour.get());

		Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);
		Texture* tex = (Texture*)go->getComponent(CT_TEXTURE);
		Collision* col = (Collision*)go->getComponent(CT_COLLISION);

		if (go)
		{
			Transform* trans = (Transform*)go->getComponent(CT_TRANSFORM);
			if (trans)
			{
				trans->setTranslate(trans->m_starting_translate);
				trans->setScale(trans->m_starting_scale);
			}

			// Reset Objects (with their original textures) - fake and real objects
			if (evidence_behave)
			{
				evidence_behave->Init();
			}

			if (npc_behave)
			{
				npc_behave->Init();
			}

			if (proximity_behave)
			{
				proximity_behave->Init();
			}

			if (ui_behave)
			{
				ui_behave->Init();
			}

			// Reset Dialogues
			if (dial && tex) // turn off dialogue elements
			{
				tex->isRendering = false;

				if (col)
				{
					col->isActive = false;
				}

			}

		}
	}

	// Reset Sprint Mode
	SprintMode = false;

	// Reset Player Holding Item
	GameObject* player_go = SEFindObjectByName("Player9");
	Behaviour_Player* player_behave = dynamic_cast<Behaviour_Player*>(player_go->m_behaviour.get());

	if (player_go)
	{
		if (player_behave)
		{
			player_behave->m_currentHoldingItem = Behaviour_Player::INTERACTABLE::NONE; // reset what he is holding. 
		}
	}

	// Fix for Lala Flip
	stopFlipping = true;

	// Fix Chasing + movement
	block_move = false;


	GameObject* gameover_go = SEFindObjectByName("Game Over Screen0");
	GameObject* retrybtn_go = SEFindObjectByName("GO Retry1");
	GameObject* exitbtn_go = SEFindObjectByName("GO Exit2");

	if (gameover_go)
	{
		Texture* gameover_text = (Texture*)gameover_go->getComponent(CT_TEXTURE);
		if (gameover_text)
		{
			gameover_text->isRendering = false;
		}
	}

	if (retrybtn_go)
	{
		Texture* retrybtn_text = (Texture*)retrybtn_go->getComponent(CT_TEXTURE);
		retrybtn_text->isRendering = false;
	}

	if (exitbtn_go)
	{
		Texture* exitbtn_text = (Texture*)exitbtn_go->getComponent(CT_TEXTURE);
		exitbtn_text->isRendering = false;
	}

	lala_stops_banging_her_head_back_and_forth = false;
}

void SET_LEVEL(LEVEL _level)
{
	current_level = _level; // sets [NPC Lala]'s route to the correct level (inside [Behaviour_NPC::Update()]

	// (TODO) - set data relating to the level given
	// 1. Set [NPC] waypoints 
	// 2. Set [Objective List]
	// 3. Set [Chase Flags]
	// 4. Set [Dialogue Flags]

	// End [Lv1] Game Progress
	if (_level != LEVEL::LEVEL_1)
	{
		for (bool& progress_bool : GameProgressLv1)
		{
			progress_bool = true;
		}

		// End [Lv1] Dialouge Progress (Muse)
		for (bool& muse_bool : DialogueMuseProgressLv1)
		{
			muse_bool = true;
		}

		// End [Lv1] Dialogue Progress (Lala)
		for (bool& lala_bool : DialogueLalaProgressLv1)
		{
			lala_bool = true;
		}

		// End [Lv1] Chase Progress
		for (bool& chase_bool : ChaseStatusLv1)
		{
			chase_bool = true;
		}
	}

}



void Behaviour_Player::Init()
{
#if _DEBUG
	std::cout << "Player init\n";
#endif
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	m_CurrentAction = ACTION::ACTION_IDLE;
	m_Type = ITEM_TYPE::PLAYER;

	m_Collision->isActive = false;

	PDef_Scale = m_Transform->getScale();
	InitVal.Player.Pos = m_Transform->getTranslate();
}

void Behaviour_Player::Update()
{

	if (scale_init)
	{
		c_xscale = m_Transform->getScale().x;
		scale_init = 0;
	}
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	assert(m_Transform); // MISSING TRANSFORM COMPONENT
	assert(m_Collision); // MISSING COLLIISION COMPONENT
	assert(m_Texture);	 // MISSING TEXTURE COMPONENT

	/*m_Collision->SetCollisionSize({ 5,5 });*/


	m_CurrentPos = m_Transform->getTranslate();

	switch (m_currentHoldingItem)
	{
	case INTERACTABLE::NONE:
		m_Transform->setScale(m_Transform->getScale().x > 0 ? -PDef_Scale.x : PDef_Scale.x, PDef_Scale.y);
		m_currentIdle = ACTION::ACTION_IDLE;
		m_currentWalking = ACTION::ACTION_WALKING;
		m_currentSearching = ACTION::ACTION_SEARCHING;
		m_currentSearching = ACTION::ACTION_SEARCH_FAKE_HORN;
		break;

	case INTERACTABLE::PAPER_FAKE:
	case INTERACTABLE::PAPER_REAL:
		m_currentIdle = ACTION::ACTION_IDLE_PAPER;
		m_currentWalking = ACTION::ACTION_WALKING_PAPER;
		break;

	case INTERACTABLE::BROOM_PUTDOWN:
	case INTERACTABLE::BROOM_PICKUP:
		m_Transform->setScale((m_Transform->getScale().x > 0.0f ? -PDef_Scale.x : PDef_Scale.x) * HOLDBROOM_SCALE, PDef_Scale.y * HOLDBROOM_SCALE);
		m_currentIdle = ACTION::ACTION_IDLE_BROOM;
		m_currentWalking = ACTION::ACTION_WALKING_BROOM;
		m_currentCleaning = ACTION::ACTION_CLEANING_BROOM;
		break;
	case INTERACTABLE::PHOTO_FROM_PILE: // fallthrough to photo
	case INTERACTABLE::PHOTO:
		m_currentIdle = ACTION::ACTION_IDLE_PHOTO;
		m_currentWalking = ACTION::ACTION_WALKING_PHOTO;
		break;

	case INTERACTABLE::TRASHBAG:
		m_currentIdle = ACTION::ACTION_IDLE_TRASHBAG;
		m_currentWalking = ACTION::ACTION_WALKING_TRASHBAG;
		break;

	case INTERACTABLE::MOP_PICKUP:
		m_currentIdle = ACTION::ACTION_IDLE_MOP;
		m_currentWalking = ACTION::ACTION_WALKING_MOP;
		break;

	case INTERACTABLE::MOP_BLOODY:
		m_currentIdle = ACTION::ACTION_IDLE_BLOODY_MOP;
		m_currentWalking = ACTION::ACTION_WALKING_BLOODY_MOP;
		break;

	case INTERACTABLE::MOP_PUTDOWN: // after washing - ready to put down
		m_currentIdle = ACTION::ACTION_IDLE_MOP;
		m_currentWalking = ACTION::ACTION_WALKING_MOP;
		break;

	case INTERACTABLE::BUCKET_FAKE_BLOOD_FULL:
		m_currentWalking = ACTION::ACTION_WALKING_FULL_PAINT_BUCKET;
		m_currentIdle = ACTION::ACTION_IDLE_FULL_PAINT_BUCKET;
		break;

	case INTERACTABLE::BUCKET_EMPTY:
		m_currentIdle = ACTION::ACTION_IDLE_EMPTY_PAINT_BUCKET;
		m_currentWalking = ACTION::ACTION_WALKING_EMPTY_PAINT_BUCKET;
		break;

	case INTERACTABLE::KNIFE_UNWASHED:
		m_currentIdle = ACTION::ACTION_IDLE_BLOODY_KNIFE;
		m_currentWalking = ACTION::ACTION_WALKING_BLOODY_KNIFE;
		break;

	case INTERACTABLE::KNIFE_WASHED:
		m_currentIdle = ACTION::ACTION_IDLE_CLEAN_KNIFE;
		m_currentWalking = ACTION::ACTION_WALKING_CLEAN_KNIFE;
		break;

	case INTERACTABLE::REAL_CANDY_HORN:
		m_currentIdle = ACTION::ACTION_IDLE_CANDY_HORN;
		m_currentWalking = ACTION::ACTION_WALKING_CANDY_HORN;
		break;
	case INTERACTABLE::FAKE_CANDY_HORN:
		m_currentIdle = ACTION::ACTION_IDLE_FAKE_CANDY_CORN;
		m_currentWalking = ACTION::ACTION_WALKING_FAKE_CANDY_CORN;
		break;
	}

	if (SEIsKeyTriggered(SE_KEY_LSHIFT))
	{
		SprintMode = !SprintMode;
	}

	if (SprintMode)
	{
		m_movementSpeed = MAX_MOVEMENT_SPEED;
	}
	else
		m_movementSpeed = MIN_MOVEMENT_SPEED;

	float deltaTime = (float)SEGetDeltaTime();
	float moveDist = m_movementSpeed * deltaTime;
	bool isWalking = false;



	if (block_move == false && currSusMLevel < 5) // during dialogue ... should not be able to move...
	{
		if (SEIsKeyHeldDown(SE_KEY_W) || SEIsKeyTriggered(SE_KEY_W))
		{
			m_CurrentAction = m_currentWalking;

			auto current_pos = m_Transform->getTranslate();
			m_CurrentPos.y = current_pos.y + moveDist;
			movedDir[0] = true;
			isWalking = true;
			// SEPlaySound(AUDIOTYPE::SFX, "./Assets/Audio/SFX/Footsteps_Track_Loop.wav");
		}
		else if (SEIsKeyHeldDown(SE_KEY_S) || SEIsKeyTriggered(SE_KEY_S))
		{
			m_CurrentAction = m_currentWalking;
			auto current_pos = m_Transform->getTranslate();
			m_CurrentPos.y = current_pos.y - moveDist;
			movedDir[1] = true;
			isWalking = true;
			// SEPlaySound(AUDIOTYPE::SFX, "./Assets/Audio/SFX/Footsteps_Track_Loop.wav");
		}

		if (SEIsKeyHeldDown(SE_KEY_A) || SEIsKeyTriggered(SE_KEY_A))
		{
			m_CurrentAction = m_currentWalking;
			auto current_pos = m_Transform->getTranslate();
			m_Transform->setScale(c_xscale, m_Transform->getScale().y); // invert x axis, to show png facing the intended direction.
			m_CurrentPos.x = current_pos.x - moveDist;
			movedDir[2] = true;
			isWalking = true;
			// SEPlaySound(AUDIOTYPE::SFX, "./Assets/Audio/SFX/Footsteps_Track_Loop.wav");
		}
		else if (SEIsKeyHeldDown(SE_KEY_D) || SEIsKeyTriggered(SE_KEY_D))
		{
			m_CurrentAction = m_currentWalking;
			auto current_pos = m_Transform->getTranslate();
			m_Transform->setScale(-c_xscale, m_Transform->getScale().y); // invert x axis, to show png facing the intended direction.
			m_CurrentPos.x = current_pos.x + moveDist;
			movedDir[3] = true;
			isWalking = true;
			// SEPlaySound(AUDIOTYPE::SFX, "./Assets/Audio/SFX/Footsteps_Track_Loop.wav");
		}
	}

	m_Transform->setTranslate(m_CurrentPos);
	SetCameraPosition(m_PreviousPos.x, m_PreviousPos.y);

	if (m_CurrentPos == m_PreviousPos)
	{
		m_CurrentAction = m_currentIdle;
	}

	if (m_CurrentPos != m_PreviousPos)
	{
		ScriptTimer.Reset();
	}

	static float walkSoundTimer = -1.f;
	float soundTime = 7.f;
	if (isWalking)
	{
		if (walkSoundTimer < 0)
		{
			SEStopSound(AUDIOTYPE::SFX, std::string{ "Footsteps_Track_Loop.wav" });
			SEPlaySound(AUDIOTYPE::SFX, "Footsteps_Track_Loop.wav");
			walkSoundTimer += soundTime;
		}
		else
		{
			walkSoundTimer -= (float)deltaTime;
		}
	}
	else
	{
		walkSoundTimer = -1.f;
		SEStopSound(AUDIOTYPE::SFX, std::string{ "Footsteps_Track_Loop.wav" });
	}

	m_Collision->SetCollision({ m_Transform->getTranslate().x, m_Transform->getTranslate().y - ((m_Transform->getScale().y / 2) + 10) }, { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });

	if (m_CurrentPos == m_PreviousPos && m_CurrentAction != ACTION::ACTION_CLEANING_BROOM && m_CurrentAction != ACTION::ACTION_SEARCHING && m_CurrentAction != ACTION::ACTION_SEARCH_FAKE_HORN)
	{
		m_CurrentAction = m_currentIdle;
	}


	//if (m_currentHoldingItem == Behaviour_Player::INTERACTABLE::MOP_BLOODY)
	//{
	//	SEDialogueToggleOn("Player9", "Dirty Mop Bubble"); 
	//}

	//else
	//{
	//	SESpecificDialogueToggleOff("Player9", "Dirty Mop Bubble");
	//}

	// Reset

	static bool trash = false;
	static bool flag = true;

	// Dialogue Boolean
	static bool start_paper_taken = false;
	static bool shown_main_menu = false;
	static bool shard_sweep = false;
	static bool photo_taken = false;
	static bool search_played = false;

	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;
		if (gameObj)
		{
			for (const auto& obj2 : goArray)
			{
				GameObject* gameObj2 = obj2.second.second;
				if (gameObj2)
				{
					std::string const& name1 = gameObj->getName();
					std::string const& name2 = gameObj2->getName();
					Collision* col1 = (Collision*)gameObj->getComponent(CT_COLLISION);
					Collision* col2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

					Texture* tex1 = (Texture*)gameObj->getComponent(CT_TEXTURE); (void)tex1;
					Texture* tex2 = (Texture*)gameObj->getComponent(CT_TEXTURE); (void)tex2;

					static bool once = false;
					if (col1 && col2)
					{
						if (SEIsKeyHeldDown(SE_KEY_Q) && SECheckCollision(col1, col2))
						{

							if (name1 == "Player9")
							{
								if (name2 == "PP Pile51" && m_currentHoldingItem == INTERACTABLE::NONE)
								{
									search_played = true;

									if (PR1_currentOccuItem == INTERACTABLE::NONE || PF25_currentOccuItem == INTERACTABLE::NONE) {
										if (ScriptTimer.Countup(2.0))
										{

											if (SEGetGameProgress(ProgressFlags_LV1::DISPOSE_REAL_NOTE) && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE))
											{
												m_currentHoldingItem = INTERACTABLE::PAPER_FAKE;

											}
											else if (SEGetGameProgress(ProgressFlags_LV1::DISPOSE_REAL_PHOTO) && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_PHOTO))
											{
												m_currentHoldingItem = INTERACTABLE::PHOTO_FROM_PILE;
											}

											search_played = false;

										}
										else
										{
											if (search_played)
											{
												//SEPlaySound(AUDIOTYPE::SFX, "Searching_Track_Loop.wav");
											}

											m_CurrentAction = ACTION::ACTION_SEARCHING;
											m_Transform->setScale((m_Transform->getScale().x > 0.0f ? -PDef_Scale.x : PDef_Scale.x) * SEARCHING_SCALE, PDef_Scale.y * SEARCHING_SCALE);
										}
									}
								}

								if (name2 == "Glass Pieces26" && m_currentHoldingItem == INTERACTABLE::BROOM_PICKUP)
								{
									if (ScriptTimer.Countup(2.0))
									{
										m_currentHoldingItem = INTERACTABLE::BROOM_PUTDOWN;
										GP26_currentOccuItem = INTERACTABLE::TRASHBAG;
										GameProgressLv1[(int)ProgressFlags_LV1::SWEEP_GLASS] = true;
										m_Transform->setScale((m_Transform->getScale().x > 0 ? -PDef_Scale.x : PDef_Scale.x));

										if (SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_14))
											ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_14);

										if (SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_9))
											ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_9);
									}

									else
									{
										m_CurrentAction = ACTION::ACTION_CLEANING_BROOM;
										m_Transform->setScale((m_Transform->getScale().x > 0.0f ? -PDef_Scale.x : PDef_Scale.x) * CLEANING_SCALE, PDef_Scale.y * CLEANING_SCALE);
									}
								}

								if (name2 == "Sink21") {
									if (m_currentHoldingItem == INTERACTABLE::MOP_BLOODY)
									{
										if (ScriptTimer.Countup(2.0))
										{
											m_currentHoldingItem = INTERACTABLE::MOP_PUTDOWN;
											S21_currentOccuItem = INTERACTABLE::NONE;

											ToggleGameProgressLv2(ProgressFlags_LV2::WASH_BLOOD_STAIN);
										}

										else {
											m_CurrentAction = ACTION::ACTION_WASHING;
											S21_currentOccuItem = INTERACTABLE::MOP_BLOODY;
										}
									}

									if (m_currentHoldingItem == INTERACTABLE::KNIFE_UNWASHED)
									{
										if (ScriptTimer.Countup(2.0))
										{
											m_currentHoldingItem = INTERACTABLE::KNIFE_WASHED;
											S21_currentOccuItem = INTERACTABLE::KNIFE_WASHED;

											//ToggleGameProgressLv2(ProgressFlags_LV2::WASH_BLOOD_STAIN);
										}

										else {
											m_CurrentAction = ACTION::ACTION_WASHING;
											S21_currentOccuItem = INTERACTABLE::KNIFE_UNWASHED;
										}
									}
								}

								if (name2 == "pinkblood3" && m_currentHoldingItem == INTERACTABLE::MOP_PICKUP)
								{
									if (ScriptTimer.Countup(2.0))
									{
										// Change Texture to Empty (to signify blood has been cleaned up)
										Behaviour_Evidence* blood_evidence = dynamic_cast<Behaviour_Evidence*>(gameObj2->m_behaviour.get());

										if (blood_evidence)
										{
											blood_evidence->m_currentAction = ACTION::ACTION_NULL;
										}

										m_currentHoldingItem = INTERACTABLE::MOP_BLOODY;
										ToggleGameProgressLv2(ProgressFlags_LV2::MOP_BLOOD_STAIN);
									}
									else
									{
										m_CurrentAction = ACTION::ACTION_CLEANING_MOP;
										m_Transform->setScale((m_Transform->getScale().x > 0.0f ? -PDef_Scale.x : PDef_Scale.x) * SEARCHING_SCALE, PDef_Scale.y * SEARCHING_SCALE);
									}

								}

								if (name2 == "pinkblood3" && m_currentHoldingItem == INTERACTABLE::BUCKET_FAKE_BLOOD_FULL)
								{
									if (ScriptTimer.Countup(2.0))
									{
										// Change Texture to Empty (to signify blood has been cleaned up)
										Behaviour_Evidence* blood_evidence = dynamic_cast<Behaviour_Evidence*>(gameObj2->m_behaviour.get());

										if (blood_evidence)
										{
											blood_evidence->m_currentAction = ACTION::ACTION_FAKE_BLOOD;
										}

										m_currentHoldingItem = INTERACTABLE::BUCKET_EMPTY;
										ToggleGameProgressLv2(ProgressFlags_LV2::POUR_FAKE_BLOOD);
									}
									else
									{
										
										m_CurrentAction = ACTION::ACTION_POURING_PAINT;
										m_Transform->setScale((m_Transform->getScale().x > 0.0f ? -PDef_Scale.x : PDef_Scale.x) * SEARCHING_SCALE, PDef_Scale.y * SEARCHING_SCALE);
									}

								}

								if (name2 == "candyPile2" && SEGetGameProgress(ProgressFlags_LV2::THROW_AWAY_HORN) && !SEGetGameProgress(ProgressFlags_LV2::REPLACE_HORN_WITH_CANDY))
								{
									if (ScriptTimer.Countup(2.0))
									{
										if (m_currentHoldingItem == INTERACTABLE::NONE)
										{
											m_currentHoldingItem = INTERACTABLE::FAKE_CANDY_HORN;
											CP_currentOccuItem = INTERACTABLE::CANDY_PILE_SEARCHED;
											SESpecificDialogueToggleOff(name1, "Candy Pile Prompt");
										}
									}
									else
									{
										//SEPlaySound(AUDIOTYPE::SFX, "Searching_Track_Loop.wav");
										m_CurrentAction = ACTION::ACTION_SEARCH_FAKE_HORN;
										m_Transform->setScale((m_Transform->getScale().x > 0.0f ? -PDef_Scale.x : PDef_Scale.x) * SEARCHING_SCALE, PDef_Scale.y * SEARCHING_SCALE);
									}
								}
							}

						}

						if (SEIsKeyReleased(SE_KEY_Q) && name1 == "Player9")
						{
							if (name2 == "PP Pile51" && SEGetGameProgress(ProgressFlags_LV1::DISPOSE_REAL_NOTE))
								SEStopSound(AUDIOTYPE::SFX, std::string("Searching_Track_Loop.wav"));
							else if (name2 == "candyPile2" && SEGetGameProgress(ProgressFlags_LV2::THROW_AWAY_HORN))
								SEStopSound(AUDIOTYPE::SFX, std::string("Searching_Track_Loop.wav"));
							else if (name2 == "Glass Pieces26" && (m_currentHoldingItem == INTERACTABLE::BROOM_PICKUP || m_currentHoldingItem == INTERACTABLE::BROOM_PUTDOWN))
								SEStopSound(AUDIOTYPE::SFX, std::string("Sweeping_Track_Loop.wav"));
							else if (name2 == "Sink21")
								SEStopSound(AUDIOTYPE::SFX, std::string("Washing_Track_Loop.wav"));
							else if(name2 == "pinkblood3" && m_currentHoldingItem == INTERACTABLE::BUCKET_FAKE_BLOOD_FULL)
								SEStopSound(AUDIOTYPE::SFX, std::string("Washing_Track_Loop.wav"));
							else if (name2 == "pinkblood3" && (m_currentHoldingItem == INTERACTABLE::MOP_PICKUP || m_currentHoldingItem == INTERACTABLE::MOP_PUTDOWN))
								SEStopSound(AUDIOTYPE::SFX, std::string("Sweeping_Track_Loop.wav"));
						}

						if (SEIsKeyTriggered(SE_KEY_Q) && SECheckCollision(col1, col2)) // col1->CollisionCheck(col2))
						{
							if (name1 == "Player9")
							{
								/*if (name2 == "Staic Jack In The Box Level 2" && m_currentHoldingItem == INTERACTABLE::NONE)
								{
									if (ScriptTimer.Countup(2.0))
									{

									}

									else
									{
										gameObj2->m_behaviour.get()
									}
								}*/


								if (name2 == "Paper Real1" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE) && !SEGetGameProgress(ProgressFlags_LV1::DISPOSE_REAL_NOTE))
								{
									m_currentHoldingItem = INTERACTABLE::PAPER_REAL;
									PR1_currentOccuItem = INTERACTABLE::NONE;
									start_paper_taken = true;

									if (shown_main_menu == false)
									{
										GameObject* how_to_play = SEFindObjectByName("HTP Bg38_Level_1");
										GameObject* how_to_play_exit = SEFindObjectByName("HTP Btn Default39_Level_1");

										if (how_to_play && how_to_play_exit)
										{
											Texture* _tex1 = (Texture*)how_to_play->getComponent(CT_TEXTURE);
											Texture* _tex2 = (Texture*)how_to_play_exit->getComponent(CT_TEXTURE);

											_tex1->isRendering = true;
											_tex2->isRendering = true;
										}

										block_move = true;
									}

									shown_main_menu = true;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									PoofVFX(gameObj2);
								}

								if (name2 == "Paper Real1" && m_currentHoldingItem == INTERACTABLE::PAPER_FAKE && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE))
								{
									m_currentHoldingItem = INTERACTABLE::NONE;
									PR1_currentOccuItem = INTERACTABLE::PAPER_FAKE;
									GameProgressLv1[(int)ProgressFlags_LV1::KEY_REPLACE_NOTE] = true;
									if (SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_1) == false)
									{
										ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_1); // set it to true since this dialogue is not needed anymore.
										ToggleChaseRecord(ChaseRecords_LV1::CHASE_FAKE_PAPER);	 // Toggle chase to mark as "completed" (more of do not need to chase anymore)
									}

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->chasing = false;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");

									proximity = SEFindObjectByName("NPC Moose Proximity");
									proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->chasing = false;
								}
								if (name2 == "PP Pile51" && m_currentHoldingItem == INTERACTABLE::NONE && SEGetGameProgress(ProgressFlags_LV1::DISPOSE_REAL_NOTE))
								{
									if (SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_1) == true)
										ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_1);

									if (SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_7) == true)
										ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_7);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->chasing = false;

									SEPlaySound(AUDIOTYPE::SFX, "Searching_Track_Loop.wav");
									proximity = SEFindObjectByName("NPC Moose Proximity");
									proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->chasing = false;
								}

								if (name2 == "Bin13" && m_currentHoldingItem == INTERACTABLE::PAPER_REAL)
								{
									m_currentHoldingItem = INTERACTABLE::NONE;
									GameProgressLv1[(int)ProgressFlags_LV1::DISPOSE_REAL_NOTE] = true;
									if (SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_1) == false)
									{
										ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_1); // set it to true since this dialogue is not needed anymore.
										ToggleChaseRecord(ChaseRecords_LV1::CHASE_REAL_PAPER);	 // Toggle chase to mark as "completed" (more of do not need to chase anymore)
									}

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->chasing = false;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									PoofVFX(gameObj2);

									proximity = SEFindObjectByName("NPC Moose Proximity");
									proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->chasing = false;
								}
								if (name2 == "Bin12" && (m_currentHoldingItem == INTERACTABLE::PHOTO || m_currentHoldingItem == INTERACTABLE::TRASHBAG))
								{
									if (m_currentHoldingItem == INTERACTABLE::TRASHBAG)
									{
										GameProgressLv1[(int)ProgressFlags_LV1::KEY_DISPOSE_GLASS] = true;
										ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_10);
										ToggleChaseRecord(ChaseRecords_LV1::CHASE_RUBBISH_BAG);
									}

									else if (m_currentHoldingItem == INTERACTABLE::PHOTO) {
										GameProgressLv1[(int)ProgressFlags_LV1::DISPOSE_REAL_PHOTO] = true;

										// Do not need to chase for real photo anymore + Do not need to open that particular dialogue anym
										ToggleChaseRecord(ChaseRecords_LV1::CHASE_REAL_PHOTO);
										ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_11);
									}

									m_currentHoldingItem = INTERACTABLE::NONE;

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									proximity_behave->chasing = false;

									proximity = SEFindObjectByName("NPC Moose Proximity");
									proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									proximity_behave->chasing = false;

									shard_sweep = true;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									PoofVFX(gameObj2);
								}

								//if (m_currentHoldingItem == INTERACTABLE::PHOTO)
								//{
								//	ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_11); // set it to true since this dialogue is not needed anymore.
								//	ToggleChaseRecord(ChaseRecords_LV1::CHASE_REAL_PHOTO);	  // Toggle chase to mark as "completed" (more of do not need to chase anymore)
								//}

								if (name2 == "Photo Frame25" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV1::DISPOSE_REAL_PHOTO))
								{
									m_currentHoldingItem = INTERACTABLE::PHOTO;
									PF25_currentOccuItem = INTERACTABLE::NONE;
									SESpecificDialogueToggleOff(name1, "Node #8"); // thought bubble.
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									PoofVFX(gameObj2);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get(); (void)proximity_behave;

									//proximity_behave->proximity_touched = false;
									photo_taken = true;
								}

								if (name2 == "Photo Frame25" && m_currentHoldingItem == INTERACTABLE::PHOTO_FROM_PILE && SEGetGameProgress(ProgressFlags_LV1::DISPOSE_REAL_PHOTO))
								{
									m_currentHoldingItem = INTERACTABLE::NONE;
									PF25_currentOccuItem = INTERACTABLE::PHOTO_FROM_PILE;
									GameProgressLv1[(int)ProgressFlags_LV1::KEY_REPLACE_PHOTO] = true;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
								}

								if (name2 == "Broom9" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV1::SWEEP_GLASS))
								{
									m_currentHoldingItem = INTERACTABLE::BROOM_PICKUP;
									B9_currentOccuItem = INTERACTABLE::NONE;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									PoofVFX(gameObj2);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									proximity_behave->chasing = false;

									proximity = SEFindObjectByName("NPC Moose Proximity");
									proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									proximity_behave->chasing = false;
								}

								if (name2 == "Broom9" && m_currentHoldingItem == INTERACTABLE::BROOM_PUTDOWN && SEGetGameProgress(ProgressFlags_LV1::SWEEP_GLASS))
								{
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									m_currentHoldingItem = INTERACTABLE::NONE;
									B9_currentOccuItem = INTERACTABLE::BROOM_PUTDOWN;
								}

								if (name2 == "Glass Pieces26" && m_currentHoldingItem == INTERACTABLE::BROOM_PICKUP)
								{
									ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_9); // set it to true since this dialogue is not needed anymore.
									ToggleChaseRecord(ChaseRecords_LV1::CHASE_BROOM);		 // Toggle chase to mark as "completed" (more of do not need to chase anymore)

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									proximity_behave->chasing = false;

									proximity = SEFindObjectByName("NPC Moose Proximity");
									proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									proximity_behave->chasing = false;

									SEPlaySound(AUDIOTYPE::SFX, "Sweeping_Track_Loop.wav");
								}

								if (name2 == "Glass Pieces26" && m_currentHoldingItem == INTERACTABLE::NONE && GP26_currentOccuItem == INTERACTABLE::TRASHBAG)
								{
									m_currentHoldingItem = INTERACTABLE::TRASHBAG;
									GP26_currentOccuItem = INTERACTABLE::NONE;

									SESpecificDialogueToggleOff(name1, "Node #5"); // thought bubble.
									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->chasing = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									PoofVFX(gameObj2);

									proximity = SEFindObjectByName("NPC Moose Proximity");
									proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									proximity_behave->chasing = false;
								}

								// LEVEL 2 - Objects
								// ---------------------------------
								if (name2 == "mop0" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV2::REPLACE_BLOOD_WITH_PAINT) && !SEGetGameProgress(ProgressFlags_LV2::REPLACE_MOP))
								{
									m_currentHoldingItem = INTERACTABLE::MOP_PICKUP;
									M0_currentOccuItem = INTERACTABLE::NONE;
									Behaviour_Evidence* mop_behave = dynamic_cast<Behaviour_Evidence*>(gameObj2->m_behaviour.get());
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
									PoofVFX(gameObj2);

									if (mop_behave)
										mop_behave->evidence_taken = true;
								}

								if (name2 == "mop0" && m_currentHoldingItem == INTERACTABLE::MOP_PUTDOWN && !SEGetGameProgress(ProgressFlags_LV2::REPLACE_BLOOD_WITH_PAINT) && !SEGetGameProgress(ProgressFlags_LV2::REPLACE_MOP))
								{
									m_currentHoldingItem = INTERACTABLE::NONE;
									M0_currentOccuItem = INTERACTABLE::MOP_PUTDOWN;
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");

									ToggleGameProgressLv2(ProgressFlags_LV2::REPLACE_MOP);
								}


								if (name2 == "Sink21")
								{
									if (S21_currentOccuItem == INTERACTABLE::SINK_ON)
									{
										S21_currentOccuItem = INTERACTABLE::SINK_OFF;
									}

									else
									{
										S21_currentOccuItem = INTERACTABLE::SINK_ON;
										SEPlaySound(AUDIOTYPE::SFX, "Washing_Track_Loop.wav");
									}

								}

								if (name2 == "Paint Bucket" && !SEGetGameProgress(ProgressFlags_LV2::PUT_BACK_EMPTY_BUCKET) && SEGetGameProgress(ProgressFlags_LV2::MOP_BLOOD_STAIN)) // This sub-objective has not been accomplish 
								{
									switch (m_currentHoldingItem)
									{
									case INTERACTABLE::NONE:
										PB_currentOccuItem = INTERACTABLE::NONE;
										m_currentHoldingItem = INTERACTABLE::BUCKET_FAKE_BLOOD_FULL; // holding bucket with fake blood
										ToggleGameProgressLv2(ProgressFlags_LV2::TAKE_PAINT_BUCKET);
										SESpecificDialogueToggleOff("Player9", "Bucket Thought Bubble");
										PoofVFX(gameObj2);
										break;
									case INTERACTABLE::BUCKET_EMPTY:
										PB_currentOccuItem = INTERACTABLE::BUCKET_EMPTY;
										m_currentHoldingItem = INTERACTABLE::NONE;
										ToggleGameProgressLv2(ProgressFlags_LV2::PUT_BACK_EMPTY_BUCKET); // complete this task. 
										break;
									}
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");

								}



								if (name2 == "Blood Knife" && !SEGetGameProgress(ProgressFlags_LV2::CLEAN_BLOOD_ON_KNIFE))
								{
									switch (m_currentHoldingItem)
									{
									case INTERACTABLE::NONE:
										BK_currentOccuItem = INTERACTABLE::NONE;
										m_currentHoldingItem = INTERACTABLE::KNIFE_UNWASHED;
										SESpecificDialogueToggleOff("Player9", "Blood Stain Knife Bubble");
										PoofVFX(gameObj2);
										break;
									case INTERACTABLE::KNIFE_WASHED:
										BK_currentOccuItem = INTERACTABLE::KNIFE_WASHED;
										m_currentHoldingItem = INTERACTABLE::NONE;
										ToggleGameProgressLv2(ProgressFlags_LV2::CLEAN_BLOOD_ON_KNIFE);
										break;
									}
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
								}

								if (name2 == "Candy Horn" && !SEGetGameProgress(ProgressFlags_LV2::REPLACE_HORN_WITH_CANDY))
								{
									switch (m_currentHoldingItem)
									{
									case INTERACTABLE::NONE:
										CH_currentOccuItem = INTERACTABLE::NONE;
										m_currentHoldingItem = INTERACTABLE::REAL_CANDY_HORN;
										PoofVFX(gameObj2);
										break;
									case INTERACTABLE::FAKE_CANDY_HORN:
										CH_currentOccuItem = INTERACTABLE::FAKE_CANDY_HORN;
										m_currentHoldingItem = INTERACTABLE::NONE;
										ToggleGameProgressLv2(ProgressFlags_LV2::REPLACE_HORN_WITH_CANDY);
										break;
									}
									SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
								}

								if (name2 == "Bin14")
								{
									if (m_currentHoldingItem == INTERACTABLE::REAL_CANDY_HORN)
									{
										m_currentHoldingItem = INTERACTABLE::NONE;
										ToggleGameProgressLv2(ProgressFlags_LV2::THROW_AWAY_HORN);
										SEPlaySound(AUDIOTYPE::SFX, "PickUp_PutDown_Item.wav");
										PoofVFX(gameObj2);
									}
								}

								if (name2 == "candyPile2" && SEGetGameProgress(ProgressFlags_LV2::THROW_AWAY_HORN))
								{
									if (m_currentHoldingItem == INTERACTABLE::NONE)
									{
										SEPlaySound(AUDIOTYPE::SFX, "Searching_Track_Loop.wav");
									}
								}


								if (name2 == "pinkblood3" && !SEGetGameProgress(ProgressFlags_LV2::MOP_BLOOD_STAIN))
								{
									if (m_currentHoldingItem == INTERACTABLE::MOP_PICKUP)
									{
										SEPlaySound(AUDIOTYPE::SFX, "Sweeping_Track_Loop.wav");
									}
								}


								if (name2 == "pinkblood3" && !SEGetGameProgress(ProgressFlags_LV2::POUR_FAKE_BLOOD))
								{
									if (m_currentHoldingItem == INTERACTABLE::BUCKET_FAKE_BLOOD_FULL)
									{
										SEPlaySound(AUDIOTYPE::SFX, "Washing_Track_Loop.wav");
									}
								}


							}





							//// Touch Cop Moose
							// if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::PAPER)
							//{
							//	SEDialogueToggleOn();					  // Toggle speech bubble

							//}
						}
					}
				}
			}
		}
	}
	m_PreviousPos = m_CurrentPos;

	static double LaLaGaspTimer = 1.0;
	static bool LaLaIsGasping = false;
	if (LaLaIsGasping)
	{
		if (LaLaGaspTimer > 0)
			LaLaGaspTimer -= deltaTime;
		else
			LaLaIsGasping = false;
	}
	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;
		if (gameObj)
		{
			std::string name = gameObj->getName();

			if (name == "Player9")
			{
				bool collided = false;
				bool PlayerInMuseChaseArea = false;
				bool PlayerInMooseChaseArea = false;
				bool PlayerInLaLaChaseArea = false;
				for (const auto& obj2 : goArray)
				{
					GameObject* gameObj2 = obj2.second.second;
					if (gameObj2)
					{
						std::string name2 = gameObj2->getName();

						// Tells us what object is player colliding onto.
						/*	Collision* col1 = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* col2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (col1 && col2)
							{
								if (SECheckCollision(col1, col2))
								{
									std::cout << name2 << std::endl;
								}
							}*/

							// [Logic] : Blocks Player from moving pass [Cop LaLa]
						if (name2 == "mop0" && m_currentHoldingItem == Behaviour_Player::INTERACTABLE::MOP_BLOODY)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // collsion->CollisionCheck(collsion2))
								{
									//collided = true;

									SEDialogueToggleOn("Player9", "Dirty Mop Bubble");
								}
								else
								{
									SEDialogueToggleOff(name);
								}
							}
						}

						if (name2 == "Candy Horn" && m_currentHoldingItem == Behaviour_Player::INTERACTABLE::REAL_CANDY_HORN)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // collsion->CollisionCheck(collsion2))
								{
									SEDialogueToggleOff(name);
								}

							}
						}

						if ((name2 == "NPC Lala45" && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE)) || gameObj2->getTag() == "Wall" || name2 == "Door to Level 2")
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // collsion->CollisionCheck(collsion2))
								{
									collided = true;

									if (name2 == "NPC Lala45" && !LaLaIsGasping)
									{
										// SEStopSound(AUDIOTYPE::SFX, std::string{ "Footsteps_Track_Loop.wav" });
										//SEPlaySound(AUDIOTYPE::SFX, "Rabbit_Gasp.wav");
										LaLaIsGasping = true;
										LaLaGaspTimer = 1.0;
									}
								}
							}
						}

						if ((name2 == "NPC Moose" && current_level == LEVEL::LEVEL_1 || gameObj2->getTag() == "Wall" || name2 == "Door to Level 2"))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // collsion->CollisionCheck(collsion2))
								{
									collided = true;

									if (name2 == "NPC Lala45" && !LaLaIsGasping)
									{
										// SEStopSound(AUDIOTYPE::SFX, std::string{ "Footsteps_Track_Loop.wav" });
										//SEPlaySound(AUDIOTYPE::SFX, "Rabbit_Gasp.wav");
										LaLaIsGasping = true;
										LaLaGaspTimer = 1.0;
									}
								}
							}
						}

						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::PAPER_REAL && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_1))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Player is colliding into NPC Muse & Holding a Paper" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #1"); // Toggle Dialogue
									ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_1);
									ToggleChaseRecord(ChaseRecords_LV1::CHASE_REAL_PAPER); // Toggle chase to mark as "completed"
									block_move = true;
									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();
									proximity_behave->chasing = false;
								}
								else
								{
									// cannotMoveRig
									// ht = false;
									// touch_once = false;
								}
							}
						}

						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::PAPER_FAKE && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_1))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Player is colliding into NPC Muse & Holding the fake Paper" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #1"); // Toggle Dialogue
									ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_1);
									ToggleChaseRecord(ChaseRecords_LV1::CHASE_FAKE_PAPER); // Toggle chase to mark as "completed"
									block_move = true;
									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();
									proximity_behave->chasing = false;
								}
							}
						}

						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::PHOTO_FROM_PILE)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
									if (touch_once_fake_paper == false)
									{
#if _DEBUG
										std::cout << "Player is colliding into NPC Muse & Holding the photo from pile." << std::endl;
#endif
										// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
										SEDialogueToggleOn(name2, "Node #1"); // Toggle Dialogue
										touch_once_fake_paper = true;
									}
								}
							}
						}

						 if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::BROOM_PUTDOWN && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_9))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
									if (touch_once_fake_paper == false)
									{
										std::cout << "NPC Muse Questions Broom" << std::endl;
										//SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
										SEDialogueToggleOn(name2, "Node #9");					  // Toggle Dialogue
										ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_9);

										block_move = true;

										GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
										Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

										if (proximity)
										{
											if (proximity_behave)
											{
												proximity_behave->StopChasing();
											}
										}
									}
								}
							}
						}

						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::PROMPT_UNCHECKED) && current_level == LEVEL::LEVEL_1)
						{
							Transform* transform = (Transform*)gameObj->getComponent(CT_TRANSFORM);
							Transform* transform2 = (Transform*)gameObj2->getComponent(CT_TRANSFORM);

							float distance_x = transform->getTranslate().x - transform2->getTranslate().x;
							float distance_y = transform->getTranslate().y - transform2->getTranslate().y;

							float distance_hypothenuse_squared = distance_x * distance_x + distance_y * distance_y;

							float range_distance = 20000;

							// std::cout << "Distance is: " << distance_hypothenuse_squared << std::endl;

							if (distance_hypothenuse_squared <= range_distance)
							{
#if _DEBUG
								std::cout << "Informing players that they still have tasks to do." << std::endl;
#endif
								// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
								SEDialogueToggleOn(name2, "Node #2"); // Toggle Dialogue
																	  // block_move = true;
							}

							else
							{
								SEDialogueToggleOff(name2); // toggle off all dialogues relating to [NPC Lala45]
							}
						}

						if (name2 == "NPC Lala45" && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE) && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_7) && m_currentHoldingItem == INTERACTABLE::PAPER_REAL)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Player is colliding into NPC Lala with a paper, gets interrogated." << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #7"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_7);
									block_move = true;
								}
							}
						}

						if (name2 == "NPC Lala45" && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE) && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_7) && m_currentHoldingItem == INTERACTABLE::PAPER_FAKE)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Player is colliding into NPC Lala with a paper, gets interrogated." << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #7"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_7);
									block_move = true;
								}
							}
						}


						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::BROOM_PUTDOWN && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_14))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with broom" << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #14"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_14);
									ToggleLalaChaseRecordLV1(ChaseLalaRecords_LV1::CHASE_BROOM_PUTDOWN);

									block_move = true;

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #14");
								}
							}
						}

						if (name2 == "Paper Real1" && m_currentHoldingItem == INTERACTABLE::NONE)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
									if (start_paper_taken == false)
									{
#if _DEBUG
										std::cout << "I'm telling the player to press 'Q' to pick up note" << std::endl;
#endif
										// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
										SEDialogueToggleOn(name, "Node #3"); // Toggle Dialogue
																			 // SEDialogueToggleOn(name, "Node #8");		// Toggle Dialogue
									}
								}

								else
								{
									// SESpecificDialogueToggleOff(name, "Node #3");
									SEDialogueToggleOff(name);
								}
							}
						}

						if (name2 == "Bin13" && m_currentHoldingItem == INTERACTABLE::PAPER_REAL)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "I'm telling the player to press 'Q' to throw away note" << std::endl;
#endif // _DEBUG

									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Node #4"); // Toggle Dialogue
								}

								else
								{
									SEDialogueToggleOff(name);
								}
							}
						}

						if (name2 == "Glass Shards Proximity" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV1::SWEEP_GLASS)) // paper taken from the start is true then check. (if not the first convo won't appear)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG	
									std::cout << "I'm telling the player to get a broom" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2);	   // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Node #5"); // Toggle Dialogue
								}
								else
								{
									SESpecificDialogueToggleOff(name, "Node #5");
								}
							}
						}

						if (name2 == "Photo Frame Proximity" && m_currentHoldingItem == INTERACTABLE::NONE && start_paper_taken == true && photo_taken == false) // paper taken from the start is true then check. (if not the first convo won't appear)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "I'm telling the player to throw away the photo frame" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Node #8"); // Toggle Dialogue
								}

								else
								{
									SESpecificDialogueToggleOff(name, "Node #8");
								}
							}
						}

						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::BROOM_PICKUP && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_9))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a broom" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #9"); // Toggle Dialogue
									ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_9);
									block_move = true;
									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();
									proximity_behave->chasing = false;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #9");
								}
							}
						}

						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::TRASHBAG && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_10))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a thrash" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #10"); // Toggle Dialogue
									ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_10);
									block_move = true;
									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();
									proximity_behave->chasing = false;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #10");
								}
							}
						}

						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::PHOTO && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_11))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a photo" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #11"); // Toggle Dialogue
									ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_11);
									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									block_move = true;
									proximity_behave->chasing = false;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #11");
								}
							}
						}

						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::PHOTO && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_13))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a photo" << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #13"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_13);
									ToggleLalaChaseRecordLV1(ChaseLalaRecords_LV1::CHASE_REAL_PHOTO);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}


									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #13");
								}
							}
						}

						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::BROOM_PICKUP && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_14))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with broom" << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #14"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_14);
									ToggleLalaChaseRecordLV1(ChaseLalaRecords_LV1::CHASE_BROOM);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #14");
								}
							}
						}

						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::TRASHBAG && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_15))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with thrash bag" << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #15"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_15);
									ToggleLalaChaseRecordLV1(ChaseLalaRecords_LV1::CHASE_RUBBISH_BAG);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}


									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #15");
								}
							}
						}

						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::TRASHBAG && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_15))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with thrash bag" << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #15"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_15);

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #15");
								}
							}
						}

						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::PHOTO_FROM_PILE && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_12))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a photo" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #12"); // Toggle Dialogue
									ToggleDialogueMuseStatus(DialogueMuseCheck::NODE_12);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();
									proximity_behave->chasing = false;

									proximity_behave->proximity_touched = false;
									proximity_behave->make_shift_alert_timer = 0;
									proximity_behave->make_shift_esclamation_timer = 0;
									block_move = true;

								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #12");
								}
							}
						}

						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::PHOTO_FROM_PILE && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_16))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a photo" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #16"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_16);
									ToggleLalaChaseRecordLV1(ChaseLalaRecords_LV1::CHASE_FAKE_PHOTO);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #16");
								}
							}
						}

						// Mop
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::MOP_PICKUP && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_20))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #20"); // Toggle Dialogue 
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_20);
									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_MOP);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #20");
								}
							}
						}

						// Mop
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::MOP_PICKUP && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_21))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #21"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_21);
									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_CLEAN_MOP);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #21");
								}
							}
						}

						// Mop
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::MOP_PICKUP && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_22))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #22"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_22);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_CLEAN_MOP);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #22");
								}
							}
						}

						// Bloody Mop
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::MOP_BLOODY && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_23))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a dirty Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #23"); // Toggle Dialogue
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_23);
									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_BLOODY_MOP);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #23");
								}
							}
						}

						// Bloody Mop
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::MOP_BLOODY && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_24))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #24"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_24);
									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_BLOODY_MOP);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #24");
								}
							}
						}

						// Bloody Mop
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::MOP_BLOODY && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_25))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #25"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_25);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_BLOODY_MOP);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #25");
								}
							}
						}

						// Filled Bucket
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::BUCKET_FAKE_BLOOD_FULL && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_26))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #26"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_26);
									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_FILLED_BUCKET);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #26");
								}
							}
						}

						// filled bucket
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::BUCKET_FAKE_BLOOD_FULL && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_27))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a dirty Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #27"); // Toggle Dialogue
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_27);
									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_FILLED_BUCKET);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #27");
								}
							}
						}

						// filled bucket
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::BUCKET_FAKE_BLOOD_FULL && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_28))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #28"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_28);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_PAINT_BUCKET);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #28");
								}
							}
						}

						// empty Bucket
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::BUCKET_EMPTY && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_30))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #30"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_30);

									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_EMPTY_BUCKET);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #30");
								}
							}
						}

						// empty bucket
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::BUCKET_EMPTY && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_29))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a dirty Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #29"); // Toggle Dialogue
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_29);
									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_EMPTY_BUCKET);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #29");
								}
							}
						}

						// empty bucket
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::BUCKET_EMPTY && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_31))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #31"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_31);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_EMPTY_BUCKET);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #31");
								}
							}
						}

						// bloody knife
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::KNIFE_UNWASHED && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_33))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #33"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_33);

									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_BLOODY_KNIFE);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #33");
								}
							}
						}

						// bloody knife
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::KNIFE_UNWASHED && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_32))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a dirty Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #32"); // Toggle Dialogue
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_32);
									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_BLOODY_KNIFE);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #32");
								}
							}
						}

						// bloody knife
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::KNIFE_UNWASHED && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_34))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #34"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_34);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_BLOODY_KNIFE);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #34");
								}
							}
						}

						// clean knife
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::KNIFE_WASHED && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_36))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #36"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_36);

									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_CLEAN_KNIFE);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #36");
								}
							}
						}

						// clean knife
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::KNIFE_WASHED && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_35))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a dirty Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #35"); // Toggle Dialogue
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_35);

									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_KNIFE);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #35");
								}
							}
						}

						// clean knife
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::KNIFE_WASHED && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_37))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #37"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_37);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_CLEAN_KNIFE);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #37");
								}
							}
						}

						// candy corn
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::FAKE_CANDY_HORN && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_39))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #39"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_39);

									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_CANDY_CORN);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;
									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #39");
								}
							}
						}

						// candy corn
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::FAKE_CANDY_HORN && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_38))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a dirty Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #38"); // Toggle Dialogue
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_38);

									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_CANDY_HORN);


									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;
									block_move = true;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #38");
								}
							}
						}

						// candy corn
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::FAKE_CANDY_HORN && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_40))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #40"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_40);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_CANDY_CORN);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #40");
								}
							}
						}

						// candy horn
						if (name2 == "NPC Moose" && m_currentHoldingItem == INTERACTABLE::REAL_CANDY_HORN && !SEGetDialogueAkaiStatus(DialogueAkaiCheck::NODE_42))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Moose/Akai sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #42"); // Toggle Dialogue
									ToggleDialogueAkaiStatus(DialogueAkaiCheck::NODE_42);
									ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2::CHASE_CANDY_HORN);

									GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #42");
								}
							}
						}

						// candy horn
						if (name2 == "NPC Muse32" && m_currentHoldingItem == INTERACTABLE::REAL_CANDY_HORN && !SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_41))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Muse sees you with a dirty Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #41"); // Toggle Dialogue
									ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2::NODE_41);

									ToggleMuseChaseRecord(ChaseMuseRecords_LV2::CHASE_CANDY_HORN);

									GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									/*		proximity_behave->proximity_touched = false;*/
									proximity_behave->chasing = false;

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #41");
								}
							}
						}

						// candy horn
						if (name2 == "NPC Lala45" && m_currentHoldingItem == INTERACTABLE::REAL_CANDY_HORN && !SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_43))
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Lala sees you with a Mop" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name2, "Node #43"); // Toggle Dialogue
									ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2::NODE_43);
									ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2::CHASE_CANDY_HORN);

									GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
									Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

									if (proximity)
									{
										if (proximity_behave)
										{
											proximity_behave->StopChasing();
										}
									}

									block_move = true;
								}
								else
								{
									SESpecificDialogueToggleOff(name2, "Node #43");
								}
							}
						}

						if (name2 == "NPC Lala45" && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE) && !SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_7) && m_currentHoldingItem == INTERACTABLE::PAPER_FAKE)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "Player is colliding into NPC Lala with a paper, gets interrogated." << std::endl;
#endif
									SEDialogueToggleOn(name2, "Node #7"); // Toggle Dialogue
									ToggleDialogueLalaStatus(DialogueLalaCheck::NODE_7);
									block_move = true;
								}
							}
						}

						if (name2 == "pinkblood3" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV2::MOP_BLOOD_STAIN)) // paper taken from the start is true then check. (if not the first convo won't appear)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "I'm telling the player to throw away the photo frame" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Blood Stain Bubble"); // Toggle Dialogue
								}

								else
								{
									SESpecificDialogueToggleOff(name, "Blood Stain Bubble");
								}
							}
						}

						if (name2 == "Blood Knife" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV2::CLEAN_BLOOD_ON_KNIFE)) // paper taken from the start is true then check. (if not the first convo won't appear)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "I'm telling the player to throw away the photo frame" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Blood Stain Knife Bubble"); // Toggle Dialogue
								}

								else
								{
									SESpecificDialogueToggleOff(name, "Blood Stain Knife Bubble");
								}
							}
						}

						if (name2 == "Paint Bucket" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV2::TAKE_PAINT_BUCKET)) // paper taken from the start is true then check. (if not the first convo won't appear)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "I'm telling the player to throw away the photo frame" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Bucket Thought Bubble"); // Toggle Dialogue
								}

								else
								{
									SESpecificDialogueToggleOff(name, "Bucket Thought Bubble");
								}
							}
						}

						if (name2 == "candyPile2" && m_currentHoldingItem == INTERACTABLE::NONE && SEGetGameProgress(ProgressFlags_LV2::THROW_AWAY_HORN)
							&& !SEGetGameProgress(ProgressFlags_LV2::CANDY_PILE_SEARCHED)) // paper taken from the start is true then check. (if not the first convo won't appear)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "I'm telling the player to throw away the photo frame" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Candy Pile Prompt"); // Toggle Dialogue
									ToggleGameProgressLv2(ProgressFlags_LV2::CANDY_PILE_SEARCHED);
								}

								else
								{
									SESpecificDialogueToggleOff(name, "Candy Pile Prompt");
								}
							}
						}

						if (name2 == "Candy Horn" && m_currentHoldingItem == INTERACTABLE::NONE && !SEGetGameProgress(ProgressFlags_LV2::THROW_AWAY_HORN)) // paper taken from the start is true then check. (if not the first convo won't appear)
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected between [NPC Muse32 & Player9] ... while holding onto the paper...
								{
#if _DEBUG
									std::cout << "I'm telling the player to throw away the photo frame" << std::endl;
#endif
									// SEDialogueTranslateAboveEntity(gameObj2); // Move the Dialogue Box ontop of the NPC.
									SEDialogueToggleOn(name, "Candy Horn Prompt"); // Toggle Dialogue
								}

								else
								{
									SESpecificDialogueToggleOff(name, "Candy Horn Prompt");
								}
							}
						}

						std::string chaseArea = name2.substr(0, 9);
						if (chaseArea == "ChaseArea")
						{
							Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
							Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

							if (collsion && collsion2)
							{
								if (SECheckCollision(collsion, collsion2)) // If collision is detected
								{
									if (name2 == "ChaseArea0" || name2 == "ChaseArea1")
										PlayerInMuseChaseArea = true;
									else if (name2 == "ChaseArea2")
										PlayerInMooseChaseArea = true;
									else if (name2 == "ChaseArea3" || name2 == "ChaseArea4")
										PlayerInLaLaChaseArea = true;
								}
							}
						}
					}
				}



				if (!PlayerInMuseChaseArea)
				{
					GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
					Behaviour_Proximity* proximity_behave = dynamic_cast<Behaviour_Proximity*>(proximity->m_behaviour.get());
					proximity_behave->proximity_touched = false;
					proximity_behave->chasing = false;
				}
				if (!PlayerInMooseChaseArea)
				{
					GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
					Behaviour_Proximity* proximity_behave = dynamic_cast<Behaviour_Proximity*>(proximity->m_behaviour.get());
					proximity_behave->proximity_touched = false;
					proximity_behave->chasing = false;
				}
				if (!PlayerInLaLaChaseArea)
				{
					GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
					Behaviour_Proximity* proximity_behave = dynamic_cast<Behaviour_Proximity*>(proximity->m_behaviour.get());
					proximity_behave->proximity_touched = false;
					proximity_behave->chasing = false;
				}

				if (collided)
				{
					auto current_pos = m_Transform->getTranslate();
					if (movedDir[0])
						m_Transform->setTranslate(SEVec2(current_pos.x, current_pos.y -= moveDist));
					if (movedDir[1])
						m_Transform->setTranslate(SEVec2(current_pos.x, current_pos.y += moveDist));
					if (movedDir[2])
						m_Transform->setTranslate(SEVec2(current_pos.x += moveDist, current_pos.y));
					if (movedDir[3])
						m_Transform->setTranslate(SEVec2(current_pos.x -= moveDist, current_pos.y));
				}
			}

			if (name == "Interactable Collect VFX")
			{
				Texture* poofTex = (Texture*)gameObj->getComponent(CT_TEXTURE);
				if (poofTex->AnimationHandler.i > poofTex->AnimationFrames[ACTION::ACTION_POOFING] - 3)
					poofTex->isRendering = false;
			}
		}
	}

	for (int i = 0; i < 4; i++)
		movedDir[i] = false;

	SEVec2 tcam = { m_Transform->getTranslate().x, m_Transform->getTranslate().y };
	const float& width = (SEFloat)SEGetCameraWidth() / 2.f;
	const float& height = (SEFloat)SEGetCameraHeight() / 2.f;

	tcam = SE_Bounded_Camera(tcam, { width, height }, WorldBB);

	SetCameraPosition(tcam.x, tcam.y);

	/*if (SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE) && SEGetGameProgress(ProgressFlags_LV1::KEY_DISPOSE_GLASS) && SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_PHOTO))
	{
		SetCameraPosition(0, 0);
		p_Scene->SetCurrentScene(LEVEL::WIN);
		return;
	}*/
}


bool SEGetGameProgress(ProgressFlags_LV1 _progress)
{
#if _DEBUG
	std::cout << (int)_progress << std::endl;
#endif // _DEBUG
	return GameProgressLv1[(int)_progress];
}


bool SEGetGameProgress(ProgressFlags_LV2 _progress)
{
#if _DEBUG
	std::cout << (int)_progress << std::endl;
#endif // _DEBUG
	return GameProgressLv2[(int)_progress];
}

void SEResetGameProgressLv1()
{
	for (int i = 0; i < MAX_progress_lv1_count; i++) {
		GameProgressLv1[i] = false;
	}
}

void ToggleGameProgressLv1(ProgressFlags_LV1 _progress)
{
	GameProgressLv1[(int)_progress] = !GameProgressLv1[(int)_progress];
}

void ToggleGameProgressLv2(ProgressFlags_LV2 _progress)
{
	GameProgressLv2[(int)_progress] = !GameProgressLv2[(int)_progress];
}

void ToggleALLGameProgress() {
	GameProgressLv1[0] = GameProgressLv1[1] = GameProgressLv1[2] = true;
	GameProgressLv2[0] = GameProgressLv2[1] = GameProgressLv2[2] = true;
}

bool SEGetDialogueMuseStatus(DialogueMuseCheck muse_dialogue)
{
	return DialogueMuseProgressLv1[(int)muse_dialogue];
}

bool SEGetDialogueMuseStatusLv2(DialogueMuseCheckLv2 muse_dialogue)
{
	return DialogueMuseProgressLv2[(int)muse_dialogue];
}

void ToggleDialogueMuseStatus(DialogueMuseCheck muse_dialogue)
{
	DialogueMuseProgressLv1[(int)muse_dialogue] = !DialogueMuseProgressLv1[(int)muse_dialogue];
}

void ToggleDialogueMuseStatusLv2(DialogueMuseCheckLv2 muse_dialogue)
{
	DialogueMuseProgressLv2[(int)muse_dialogue] = !DialogueMuseProgressLv2[(int)muse_dialogue];
}

bool SEGetDialogueLalaStatus(DialogueLalaCheck lala_dialogue)
{
	return DialogueLalaProgressLv1[(int)lala_dialogue];
}

bool SEGetDialogueLalaStatusLv2(DialogueLalaCheckLv2 lala_dialogue)
{
	return DialogueLalaProgressLv2[(int)lala_dialogue];
}

void ToggleDialogueLalaStatus(DialogueLalaCheck lala_dialogue)
{
	DialogueLalaProgressLv1[(int)lala_dialogue] = !DialogueLalaProgressLv1[(int)lala_dialogue];
}

void ToggleDialogueLalaStatusLv2(DialogueLalaCheckLv2 lala_dialogue)
{
	DialogueLalaProgressLv2[(int)lala_dialogue] = !DialogueLalaProgressLv2[(int)lala_dialogue];
}

bool SEGetDialogueAkaiStatus(DialogueAkaiCheck Akai_dialogue)
{
	return DialogueAkaiProgress[(int)Akai_dialogue];
}

void ToggleDialogueAkaiStatus(DialogueAkaiCheck Akai_dialogue)
{
	DialogueAkaiProgress[(int)Akai_dialogue] = !DialogueAkaiProgress[(int)Akai_dialogue];
}

bool SEGetChaseRecord(ChaseRecords_LV1 chase_status)
{
	return ChaseStatusLv1[(int)chase_status];
}

bool SEGetChaseLalaRecord(ChaseLalaRecords_LV1 chase_lala_status)
{
	return LalaChaseStatusLV1[(int)chase_lala_status];
}

void ToggleLalaChaseRecordLV1(ChaseLalaRecords_LV1 chase_lala_status)
{
	LalaChaseStatusLV1[(int)chase_lala_status] = !LalaChaseStatusLV1[(int)chase_lala_status];
}

bool SEGetChaseRecord(ChaseMuseRecords_LV2 chase_status)
{
	return MuseChaseStatusLv2[(int)chase_status];
}

bool SEGetChaseRecord(ChaseAkaiRecords_LV2 chase_status)
{
	return AkaiChaseStatusLv2[(int)chase_status];
}


bool SEGetChaseRecord(ChaseLalaRecords_LV2 chase_status)
{
	return LalaChaseStatusLv2[(int)chase_status];
}


void ToggleChaseRecord(ChaseRecords_LV1 chase_status)
{
	ChaseStatusLv1[(int)chase_status] = !ChaseStatusLv1[(int)chase_status];
}

void PoofVFX(GameObject* poofObj)
{
	GameObject* poof = SEFindObjectByName("Interactable Collect VFX");
	Transform* poofTrans = (Transform*)poof->getComponent(CT_TRANSFORM);
	Transform* objTrans = (Transform*)poofObj->getComponent(CT_TRANSFORM);
	Texture* poofTex = (Texture*)poof->getComponent(CT_TEXTURE);
	poofTrans->setTranslate(objTrans->getTranslate());
	poofTex->isRendering = true;
	poofTex->AnimationHandler.i = 0;
}

//void ToggleChaseRecord(ChaseRecords_LV2 chase_status)
//{
//	ChaseStatusLv2[(int)chase_status] = !ChaseStatusLv2[(int)chase_status];
//}

void ToggleMuseChaseRecord(ChaseMuseRecords_LV2 chase_status)
{
	MuseChaseStatusLv2[(int)chase_status] = !MuseChaseStatusLv2[(int)chase_status];
}

void ToggleAkaiChaseRecord(ChaseAkaiRecords_LV2 chase_status)
{
	AkaiChaseStatusLv2[(int)chase_status] = !AkaiChaseStatusLv2[(int)chase_status];
}

void ToggleLalaChaseRecordLV2(ChaseLalaRecords_LV2 chase_status)
{
	LalaChaseStatusLv2[(int)chase_status] = !LalaChaseStatusLv2[(int)chase_status];
}

void ToggleChaseStatusLv1(ChaseRecords_LV1 chase_status);

void SEBehaviourUpdate(const LEVEL& _level)
{
	// only updates based on current Level
	for (const auto& obj : goArray)
	{
		GameObject* go = obj.second.second;
		if (go)
		{
			if (go->m_level != _level)
				continue;
			Behaviour* p_behaviour = go->m_behaviour.get();
			if (p_behaviour)
			{
				go->m_behaviour->Update();
			}
		}
	}
}

void Behaviour_Button::Init()
{
	creditsqueue = 1;
	CameraDefaultHeight = (float)SEGetCameraHeight();
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	if (m_Collision)
	{
		m_Collision->collisionPos.pos = m_Transform->getTranslate();
	}

	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);

	m_Dialogue = (Dialogue*)go->getComponent(CT_DIALOGUE);

	if (m_Dialogue)
	{
		suspicion_value = m_Dialogue->suspicion_value;
		parent_npc = m_Dialogue->parent_npc;
	}

	is_clicked = false;

	if (go->getName() == "UI Arrow")
		m_CurrentAction = ACTION::ACTION_INACTIVE;

	if (go->getName() == "GO Retry1")
		m_Collision->isActive = false;

	if (go->getName() == "GO Exit2")
		m_Collision->isActive = false;

	if (go->getName() == "Resolution Option")
		m_CurrentAction = ACTION::ACTION_INACTIVE;

	if (go->getName() == "WIN MM BTN2")
		m_Collision->isActive = false;

	/*if (go->getName() == "")
		m_Collision->isActive = false;*/
}

const float MIN_VOL = 0.0f;
const float MAX_VOL = 1.0f;
static float make_shift_timer = 0;
static bool sus_value_changed = false;
inline void DecreaseVolumeBGM(const float& value)
{
	static float Volume = SEGetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg");

	if (Volume < MIN_VOL) Volume = MIN_VOL;
	if (Volume > MAX_VOL) Volume = MAX_VOL;
	if (Volume >= MIN_VOL && Volume <= MAX_VOL)
	{
		SESetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg", Volume -= (value * float(SEGetDeltaTime())));
	}
	std::cout << "decreasing " << Volume << std::endl;
}

inline void IncreaseVolumeBGM(const float& value)
{
	static float Volume = SEGetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg");
	if (Volume < MIN_VOL) Volume = MIN_VOL;
	if (Volume > MAX_VOL) Volume = MAX_VOL;
	if (Volume >= MIN_VOL && Volume <= MAX_VOL)
	{
		SESetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg", Volume += (value * float(SEGetDeltaTime())));
	}
	std::cout << "increasing " << Volume << std::endl;
}

inline void DecreaseVolumeSFX(const float& value)
{
	auto& sfx = SEGetAudioSFXResources();

	for (auto& track : sfx)
	{
		float Volume = SEGetVolume(AUDIOTYPE::SFX, track.first);
		if (Volume < MIN_VOL) Volume = MIN_VOL;
		if (Volume > MAX_VOL) Volume = MAX_VOL;
		if (Volume >= MIN_VOL && Volume <= MAX_VOL)
		{
			SESetVolume(AUDIOTYPE::SFX, track.first, Volume -= (value * float(SEGetDeltaTime())));
			std::cout << "asdas1\n";
		}
	}
}

inline void IncreaseVolumeSFX(const float& value)
{
	auto& sfx = SEGetAudioSFXResources();

	for (auto& track : sfx)
	{
		float Volume = SEGetVolume(AUDIOTYPE::SFX, track.first);
		if (Volume < MIN_VOL) Volume = MIN_VOL;
		if (Volume > MAX_VOL) Volume = MAX_VOL;
		if (Volume >= MIN_VOL && Volume <= MAX_VOL)
		{
			SESetVolume(AUDIOTYPE::SFX, track.first, Volume += (value * float(SEGetDeltaTime())));
			std::cout << "asdas2\n";
		}
	}
}

enum MODE
{
	FULLSCREEN = 0,
	WINDOWED = 1
};
static float current_volume_from_slider{ 1.0f };
static float xposition{};
float current_volume_from_slider_scale_y{};
int current_resolution = 1;
MODE mode{ MODE::FULLSCREEN };
Texture* resolution_option_texture{};
std::filesystem::path resolution_option_texture_file{};
void Behaviour_Button::Update()
{
	current_volume_from_slider = SEgetGlobalVolume();
	SELoadTexture("./Assets/Textures\\UI_Settings_Bar_Full.png");
	auto id_settings_bar = SEGetTexture("UI_Settings_Bar_Full.png");
	auto scale = m_Transform->getScale();
	auto angle = m_Transform->getRotationAngle(); (void)angle;
	static glm::vec2 pos{};
	auto MousePosition = SEGetCursorInWorld();
	glm::vec2 Mouse = { MousePosition.x, MousePosition.y };

	if (p_Scene->GetCurrentScene() == LEVEL::LEVEL_1) {
		SESetCameraHeight(450);
	}

	else {
		SESetCameraHeight(CameraDefaultHeight);
	}
	// m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });

	Behaviour_GameplayUI* bgui = nullptr;

	GameObject* gamob = SEFindObjectByName("Sus0");

	if (gamob)
	{
		bgui = (Behaviour_GameplayUI*)gamob->m_behaviour.get();
	}

	if (currSusMLevel == prevSusMLevel && sus_value_changed == true) // make it stop glowing
	{
		if (make_shift_timer > 250)
		{
			sus_value_changed = false;
			bgui->SusStopGlow();
			make_shift_timer = 0;
		}
		else
		{
			make_shift_timer++;
		}
	}

	// How to Play UI
	// --------------------------------------------------------
	GameObject* HTP_exit_button = SEFindObjectByName("HTP Btn Default39_Level_1");
	GameObject* HTBG = SEFindObjectByName("HTP Bg38_Level_1");
	Transform* HTP_xform = (Transform*)HTBG->getComponent(CT_TRANSFORM);

	// Translate this button with respect to scale of HTP BG
	if (HTP_exit_button)
	{
		Transform* exit_trans = (Transform*)HTP_exit_button->getComponent(CT_TRANSFORM);
		Collision* exit_col = (Collision*)HTP_exit_button->getComponent(CT_COLLISION);

		SEVec2 HTP_Translate{};
		if (HTP_xform) {
			HTP_Translate = HTP_xform->getTranslate();
		}

		if (exit_trans)
		{
			exit_trans->setTranslate({ HTP_Translate.x + (-314)  , HTP_Translate.y + (160) });
		}

		if (exit_col)
		{
			exit_col->SetCollision(exit_trans->getTranslate(), { exit_col->boundaries.scale.x, exit_col->boundaries.scale.y });
		}
	}

	GameObject* go_retry = SEFindObjectByName("GO Retry1");
	if (go_retry)
	{
		Transform* retry_trans = (Transform*)go_retry->getComponent(CT_TRANSFORM);
		Collision* retry_col = (Collision*)go_retry->getComponent(CT_COLLISION);

		if (retry_trans)
		{
			retry_trans->setTranslate(SEVec2(GetCameraPositionX(), GetCameraPositionY() - 25));
		}

		if (retry_col)
		{
			retry_col->SetCollision(retry_trans->getTranslate(), { retry_col->boundaries.scale.x, retry_col->boundaries.scale.y });
		}
	}

	GameObject* go_exit = SEFindObjectByName("GO Exit2");
	if (go_exit)
	{
		Transform* exit_trans = (Transform*)go_exit->getComponent(CT_TRANSFORM);
		Collision* exit_col = (Collision*)go_exit->getComponent(CT_COLLISION);

		if (exit_trans)
		{
			exit_trans->setTranslate(SEVec2(GetCameraPositionX(), GetCameraPositionY() - 75));
		}

		if (exit_col)
		{
			exit_col->SetCollision(exit_trans->getTranslate(), { exit_col->boundaries.scale.x, exit_col->boundaries.scale.y });
		}
	}

	GameObject* go_win = SEFindObjectByName("WIN MM BTN2");
	if (go_win)
	{
		Transform* retry_trans = (Transform*)go_win->getComponent(CT_TRANSFORM);
		Collision* retry_col = (Collision*)go_win->getComponent(CT_COLLISION);

		if (retry_trans)
		{
			retry_trans->setTranslate(SEVec2(GetCameraPositionX(), GetCameraPositionY() - 120));
		}

		if (retry_col)
		{
			retry_col->SetCollision(retry_trans->getTranslate(), { retry_col->boundaries.scale.x, retry_col->boundaries.scale.y });
		}
	}

	/*switch (current_resolution)
	{
	case 0:
		SESetWindowWidthHeight(1280, 720);
		std::cout << "change to 720p\n";
		break;
	case 1:
		SESetWindowWidthHeight(1920, 1080);
		std::cout << "change to 1080p\n";
		break;
	case 2:
		SESetWindowWidthHeight(2560, 1440);
		std::cout << "change to 1440p\n";
		break;
	default:
		std::cout << "defa\n";
		break;
	}*/
	static float offset = 18.0f;
	if (go->getName() == "Button")
	{
		const float min = -75.0f;
		const float max = 593.f;
		//if (current_volume_from_slider < 0.0f)current_volume_from_slider = 0.0f;
		//if (current_volume_from_slider > 1.0f) current_volume_from_slider = 1.0f;

		xposition = current_volume_from_slider  * (max-min) + min;
		if (xposition < min)
		{
			xposition = min;
		}
		if (xposition > max)
		{
			xposition = max;
		}
		if (xposition <= min)
			offset = 0.0f;
		else
			offset = 18.0f;
		//std::cout << m_Transform->getTranslate() << std::flush;

		//current_volume_from_slider = m_Transform->getTranslate().x / (max - min);
		current_volume_from_slider_scale_y = m_Transform->getScale().y / 2;

		SEDrawSlider(-75, -145, (xposition - min - offset) , current_volume_from_slider_scale_y, 0, id_settings_bar);
		
	}

	if (current_resolution < 0) current_resolution = 1;
	if (current_resolution > 1) current_resolution = 0;
	if (current_resolution == 0) mode = MODE::WINDOWED;
	else if (current_resolution == 1) mode = MODE::FULLSCREEN;

	if (m_Collision)
	{
		if (SECheckCollision(m_Collision, Mouse) && SEIsMouseLeftPressed())
		{
			if (go->getName() == "Left Arrow")
			{
				current_resolution--;
			}
			else if (go->getName() == "Right Arrow")
			{
				current_resolution++;
			}
		}



		if (SECheckCollision(m_Collision, Mouse) && SEIsMouseLeftHeld()) // m_Collision->CollisionCheck(Mouse))
		{
			if (go->getName() == "Button")
			{
				const float min = -75.0f;
				const float max = 593.f;


				//xposition = current_volume_from_slider * max;
				if (MousePosition.x <= min)MousePosition.x = min;
				if (MousePosition.x >= max)MousePosition.x = 593.f;
				//if (MousePosition.x > min && MousePosition.x < max)
				//{
					//m_Transform->setTranslate({ xposition, m_Transform->getTranslate().y });
				m_Transform->setTranslate({ MousePosition.x, m_Transform->getTranslate().y });
				m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
				//m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
				current_volume_from_slider = (m_Transform->getTranslate().x - min) / (max - min);
				current_volume_from_slider_scale_y = m_Transform->getScale().y / 2;
				if (current_volume_from_slider < 0.0f)current_volume_from_slider = 0.0f;
				if (current_volume_from_slider > 1.0f) current_volume_from_slider = 1.0f;

				//SEDrawSlider(-75, -145, (current_volume_from_slider * max), current_volume_from_slider_scale_y, 0, id_settings_bar);
				SESetAllVolume(current_volume_from_slider);

				//SESetVolume(AUDIOTYPE::BGM, "menu.ogg", current_volume_from_slider);
			//}

				GameObject* go_whitebar = SEFindObjectByName("Empty Bar BGM");
				if (go_whitebar)
				{
				}
				/*if ( go_whitebar)
				{
					Transform* whitebar_transform = (Transform*)go_whitebar->getComponent(CT_TRANSFORM);
					if (whitebar_transform)
					{
						pos.x = whitebar_transform->getTranslate().x - 250.f;
						pos.y = whitebar_transform->getTranslate().y;
					}
				}*/


			}
			else if (go->getName() == "SFX Button")
			{
				const float min = 130.0f;
				const float max = 623.33f;
				if (MousePosition.x > min && MousePosition.x < max)
				{
					m_Transform->setTranslate({ MousePosition.x, m_Transform->getTranslate().y });
					m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
				}
			}

		}
	}

	//// decrease bgm volume
	//if (SEIsKeyTriggered(SE_KEY_LEFT))
	//{
	//	auto vol = SEGetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg");
	//	if (vol < 0.1f) vol = 0.1f;
	//	if (vol > 0.9f) vol = 0.9f;
	//	SESetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg", vol - (0.1f * SEGetDeltaTime()));
	//}

	//// increase bgm volume
	//if (SEIsKeyTriggered(SE_KEY_RIGHT))
	//{
	//	auto vol = SEGetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg");
	//	if (vol < 0.1f) vol = 0.1f;
	//	if (vol > 0.9f) vol = 0.9f;
	//	SESetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg", vol + (0.1f * SEGetDeltaTime()));
	//}

	//// decrease sfx volume
	//if (SEIsKeyTriggered(SE_KEY_DOWN))
	//{
	//	auto sfx = SEGetAudioSFXResources();
	//	for (auto& track : sfx)
	//	{
	//		auto vol = SEGetVolume(AUDIOTYPE::SFX, track.first);
	//		if (vol < 0.1f) vol = 0.1f;
	//		if (vol > 0.9f) vol = 0.9f;
	//		SESetVolume(AUDIOTYPE::SFX, track.first, vol - (0.1f * SEGetDeltaTime()));
	//		std::cout << track.first << ": " << vol << std::endl;
	//	}
	//}

	//// increase sfx volume
	//if (SEIsKeyTriggered(SE_KEY_UP))
	//{
	//	auto sfx = SEGetAudioSFXResources();
	//	for (auto& track : sfx)
	//	{
	//		auto vol = SEGetVolume(AUDIOTYPE::SFX, track.first);
	//		if (vol < 0.1f) vol = 0.1f;
	//		if (vol > 0.9f) vol = 0.9f;
	//		SESetVolume(AUDIOTYPE::SFX, track.first, vol + (0.1f * SEGetDeltaTime()));
	//		std::cout << vol << std::endl;
	//	}
	//}

	//std::cout << SEGetVolume(AUDIOTYPE::BGM, "backgroundmusic.ogg") << std::endl;
	//// Decrease SFX
	//if (SEIsKeyTriggered(SE_KEY_O))
	//{
	//	float reduction_dec_sfx = 0.0f;
	//	reduction_dec_sfx += (0.1f * SEGetDeltaTime());
	//	DecreaseVolumeSFX(reduction_dec_sfx);
	//}

	//// Increase SFX
	//if (SEIsKeyTriggered(SE_KEY_P))
	//{
	//	static float reduction_dec_sfx = 0.0f;
	//	reduction_dec_sfx -= (0.1f * SEGetDeltaTime());
	//	IncreaseVolumeSFX(reduction_dec_sfx);
	//}

	//SEDrawSlider(pos.x, pos.y, Volume, scale.y, angle, id);

	//if (go->getName() == "GO Exit2")
	//{
	//	if (m_Transform)
	//	{
	//		m_Transform->setTranslate(SEVec2(current_CameraPos.x, current_CameraPos.y));
	//	}

	//	if (m_Texture)
	//	{
	//		m_Texture->isRendering = true;
	//	}
	//}




	Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);
	Collision* col = (Collision*)go->getComponent(CT_COLLISION);

	if (m_Collision)
	{
		glm::vec2 _Mouse = { MousePosition.x, MousePosition.y };

		if (SECheckCollision(m_Collision, _Mouse)) // m_Collision->CollisionCheck(Mouse))
		{

			m_CurrentAction = ACTION::ACTION_BUTTON_ISHOVERED;

			if (SEIsMouseLeftPressed() && m_Collision->isActive)
			{
				if (!dial)
					SEPlaySound(AUDIOTYPE::SFX, "bubblepop.mp3");

				// main menu -> level 1
				if (go->getName() == "Main Menu Start Button0")
				{
					p_Scene->SetCurrentScene(LEVEL::CUTSCENE);
					//p_Scene->SetCurrentScene(LEVEL::LEVEL_1);
					// nextScene = "Level_1.json";
				}

				// main menu -> how to play
				else if (go->getName() == "Main Menu HowToPlay Button2")
				{
					p_Scene->SetCurrentScene(LEVEL::HOW_TO_PLAY);
					// nextScene = "How_To_Play_Menu.json";
				}

				// how to play (main menu) -> main menu
				else if (go->getName() == "Exit Btn Default39")
				{
					auto prev = p_Scene->GetPreviousScene();
					p_Scene->SetCurrentScene(prev);
					// nextScene = "MainMenu.json";
				}

				if (go->getName() == "Main Menu Credits Button3") {
					p_Scene->SetCurrentScene(LEVEL::CREDITS);
				}

				// main menu -> exit (prompt)
				else if (go->getName() == "Main Menu Exit Button4")
				{
					p_Scene->SetCurrentScene(LEVEL::EXIT);
					button_route_state = ButtonRoute::EXIT_PROMPT; // set this as flag for exit confirmation buttons to check.
				}

				// exit (prompt, main menu) -> click yes -> exit apps
				else if (go->getName() == "Yes Btn Default1")
				{
					if (button_route_state == ButtonRoute::EXIT_PROMPT)
					{
						// exits game, clean all memory. Create flag
						playerquit = true;
						button_route_state = ButtonRoute::NONE;

						GameObject* gameover_go = SEFindObjectByName("Game Over Screen0");
						GameObject* retrybtn_go = SEFindObjectByName("GO Retry1");
						GameObject* exitbtn_go = SEFindObjectByName("GO Exit2");

						if (gameover_go)
						{
							Texture* gameover_text = (Texture*)gameover_go->getComponent(CT_TEXTURE);
							if (gameover_text)
							{
								gameover_text->isRendering = false;
							}
						}

						if (retrybtn_go)
						{
							Texture* retrybtn_text = (Texture*)retrybtn_go->getComponent(CT_TEXTURE);
							retrybtn_text->isRendering = false;
						}

						if (exitbtn_go)
						{
							Texture* exitbtn_text = (Texture*)exitbtn_go->getComponent(CT_TEXTURE);
							exitbtn_text->isRendering = false;
						}
					}

					if (button_route_state == ButtonRoute::RESTART_PROMPT)
					{
						p_Scene->SetCurrentScene(LEVEL::LEVEL_1);
						RESET_LEVEL(LEVEL::LEVEL_1);
						button_route_state = ButtonRoute::NONE;
					}


					if (button_route_state == ButtonRoute::RETRY_PROMPT)
					{
						p_Scene->SetCurrentScene(LEVEL::LEVEL_1);
						RESET_LEVEL(LEVEL::LEVEL_1);
						button_route_state = ButtonRoute::NONE;

					}

					if (button_route_state == ButtonRoute::MAINMENU_PROMPT)
					{
						p_Scene->SetCurrentScene(LEVEL::MAIN_MENU);
						RESET_LEVEL(LEVEL::LEVEL_1);
						button_route_state = ButtonRoute::NONE;

						GameObject* gameover_go = SEFindObjectByName("Game Over Screen0");
						GameObject* retrybtn_go = SEFindObjectByName("GO Retry1");
						GameObject* exitbtn_go = SEFindObjectByName("GO Exit2");

						if (gameover_go)
						{
							Texture* gameover_text = (Texture*)gameover_go->getComponent(CT_TEXTURE);

							if (gameover_text)
							{
								gameover_text->isRendering = false;
							}
						}

						if (retrybtn_go)
						{
							Texture* retrybtn_text = (Texture*)retrybtn_go->getComponent(CT_TEXTURE);
							Collision* retrybtn_col = (Collision*)retrybtn_go->getComponent(CT_COLLISION);
							retrybtn_text->isRendering = false;
							retrybtn_col->isActive = false;
						}

						if (exitbtn_go)
						{
							Texture* exitbtn_text = (Texture*)exitbtn_go->getComponent(CT_TEXTURE);
							Collision* exitbtn_col = (Collision*)exitbtn_go->getComponent(CT_COLLISION);
							exitbtn_text->isRendering = false;
							exitbtn_col->isActive = false;
						}

						button_route_state = ButtonRoute::NONE;

					}
				}

				// exit (prompt, main menu) -> click no -> main menu
				else if (go->getName() == "No Btn Default2")
				{
					// nextScene = "MainMenu.json";
					auto prev = p_Scene->GetPreviousScene();
					p_Scene->SetCurrentScene(prev);

					GameObject* gameover_go = SEFindObjectByName("Game Over Screen0");
					GameObject* retrybtn_go = SEFindObjectByName("GO Retry1");
					GameObject* exitbtn_go = SEFindObjectByName("GO Exit2");

					if (button_route_state == ButtonRoute::RETRY_PROMPT)
					{

						if (gameover_go)
						{
							Texture* gameover_text = (Texture*)gameover_go->getComponent(CT_TEXTURE);

							if (gameover_text)
							{
								gameover_text->isRendering = true;
							}
						}

						if (retrybtn_go)
						{
							Texture* retrybtn_text = (Texture*)retrybtn_go->getComponent(CT_TEXTURE);
							Collision* retrybtn_col = (Collision*)retrybtn_go->getComponent(CT_COLLISION);
							retrybtn_text->isRendering = true;
							retrybtn_col->isActive = true;
						}

						if (exitbtn_go)
						{
							Texture* exitbtn_text = (Texture*)exitbtn_go->getComponent(CT_TEXTURE);
							Collision* exitbtn_col = (Collision*)exitbtn_go->getComponent(CT_COLLISION);
							exitbtn_text->isRendering = true;
							exitbtn_col->isActive = true;
						}
					}
					// nextScene = p_Scene->GetPreviousScene();
				}

				// main menu -> settings
				else if (go->getName() == "Main Menu Settings Button1")
				{
					p_Scene->SetCurrentScene(LEVEL::SETTINGS);
					// nextScene = "Settings_Menu.json";
				}

				// settings (main menu) -> main menu
				else if (go->getName() == "Exit button default2")
				{
					p_Scene->SetCurrentScene(LEVEL::MAIN_MENU);
					// nextScene = "MainMenu.json";
				}

				// pause menu (in game) -> resume -> lvl 1
				else if (go->getName() == "PM Resume Default1")
				{
					p_Scene->SetCurrentScene(LEVEL::LEVEL_1);
					// nextScene = "Level_1.json";
					// SELoad(positions, goArray);
				}

				// pause menu (in game) -> how to play
				else if (go->getName() == "PM HTP Default2")
				{
					p_Scene->SetCurrentScene(LEVEL::HOW_TO_PLAY);
					// nextScene = "How_To_Play_Menu.json";
				}

				// pause menu (in game) -> restart
				else if (go->getName() == "PM Restart Default4")
				{
					button_route_state = ButtonRoute::RESTART_PROMPT;
					p_Scene->SetCurrentScene(LEVEL::EXIT);
				}

				// pause menu (in game) -> main menu
				else if (go->getName() == "PM MM Default5")
				{
					p_Scene->SetCurrentScene(LEVEL::EXIT);
					button_route_state = ButtonRoute::MAINMENU_PROMPT;
				}

				else if (go->getName() == "WIN MM BTN2") {
					p_Scene->SetCurrentScene(LEVEL::ENDING);
					RESET_LEVEL(LEVEL::LEVEL_1);

					GameObject* win_btn_go = SEFindObjectByName("WIN MM BTN2");
					GameObject* win_screen_go = SEFindObjectByName("Win Screen");

					if (win_btn_go)
					{
						Texture* win_btn_tex = (Texture*)win_btn_go->getComponent(CT_TEXTURE);
						Collision* win_btn_col = (Collision*)win_btn_go->getComponent(CT_COLLISION);

						if (win_btn_tex)
						{
							win_btn_tex->isRendering = false;
						}

						if (win_btn_col)
						{
							win_btn_col->isActive = false;
						}
					}

					if (win_screen_go)
					{
						Texture* win_screen_tex = (Texture*)win_screen_go->getComponent(CT_TEXTURE);
						if (win_screen_tex)
						{
							win_screen_tex->isRendering = false;
						}
					}

				}

				else if ((go->getName() == "Officer Moose Option" || go->getName() == "Secure Bag Option" ||
					go->getName() == "Found a Paper Option" || go->getName() == "Personal Notes Option" ||
					go->getName() == "Just Checking Option" || go->getName() == "Bringing in Test Option" ||
					go->getName() == "Shifting to one side Option" || go->getName() == "Forensics Option" ||
					go->getName() == "Bring back photo Option" || go->getName() == "Got a clue Option" ||
					go->getName() == "Muse Questions Fake Photo" || go->getName() == "Send Evidence Option" ||
					go->getName() == "Found it on table Option" || go->getName() == "Officer Gave Me Option" ||
					go->getName() == "Broom Blocking Option" || go->getName() == "Broom Suspicious Option" ||
					go->getName() == "Thrash Check Option" || go->getName() == "Pass to Muse Option" ||
					go->getName() == "Photo Pass to Muse Option" || go->getName() == "Checking Fake Photo Excuse Option"
					|| go->getName() == "Pass to Lala Option" || go->getName() == "It might be potential evidence."
					|| go->getName() == "I'm placing this aside." || go->getName() == "I'm going to report this item." ||
					go->getName() == "Just going to bring it to the store room." || go->getName() == "I'm going to get this checked." ||
					go->getName() == "I want to use it to clean something up." || go->getName() == "I don't know.. I found it like this." ||
					go->getName() == "It's dirty because I was using it." || go->getName() == "Looks like the suspect tried to hide the blood!" ||
					go->getName() == "No, I used this to mop up paint." || go->getName() == "No that was a different mop." || go->getName() == "Yeah but I decided to mop up some paint." ||
					go->getName() == "Officer Muse asked me to bring it to him." || go->getName() == "I want to paint my walls at home with this." ||
					go->getName() == "It looks like blood. I want to get it checked." || go->getName() == "It's so pretty, don't you think?" ||
					go->getName() == "I want to check if it's blood." || go->getName() == "I just brought it here to show you!" || go->getName() == "That was a different bucket." ||
					go->getName() == "I already used up all the paint." || go->getName() == "No, that was another bucket." || go->getName() == "Yeah but I threw it all out." ||
					go->getName() == "I want to get this bucket checked just in case." || go->getName() == "Just bringing this to the store room." || go->getName() == "Yeah. I'll go get it checked." ||
					go->getName() == "Nah, looks like nothing to me." || go->getName() == "I'm bring it to forensics." || go->getName() == "Gonna get it all cleaned up!" ||
					go->getName() == "I'm going to turn it in as evidence." || go->getName() == "Just bringing it somewhere else.." || go->getName() == "That was a different knife."
					|| go->getName() == "Yep, I cleaned it." || go->getName() == "No, it was clean when I found it." || go->getName() == "Yeah, it was too dirty." ||
					go->getName() == "No, that was another knife." || go->getName() == "Yeah, but I decided to clean it." || go->getName() == "Back to the candy pile!" ||
					go->getName() == "I'm going to snack on this." || go->getName() == "Yeah. I brought it from home for lunch." || go->getName() == "Yeah you can take some from the pile!"
					|| go->getName() == "Just a piece of candy I brought." || go->getName() == "Candy I found on the floor." || go->getName() == "Don't worry, it's candy corn." ||
					go->getName() == "Definitely not a horn..." || go->getName() == "It might be old and expired." || go->getName() == "There's nothing weird about it!!" ||
					go->getName() == "Yeah. I'm going to get it checked." || go->getName() == "No... It's a hundred percent candy!") &&
					col->isActive)

				{
#if _DEBUG
					std::cout << "OPTION : " << go->getName() << std::endl;
#endif
					suspicion_value = m_Dialogue->suspicion_value;
					currSusMLevel += suspicion_value;
					// invoke animation to glow
					if (currSusMLevel != prevSusMLevel)
					{
						//bgui->SusGlow();
						Sus_Increase = true;
						SEPlaySound(AUDIOTYPE::SFX, "ArrowTurn.wav");
						prevSusMLevel = currSusMLevel;
					}
#if _DEBUG
					std::cout << "Suspicious Value Increased by: " << suspicion_value << std::endl;
					std::cout << "Sus Metre is now at: " << currSusMLevel << std::endl;
#endif
					SEDialogueToggleOff(m_Dialogue->parent_npc);
					GameObject* esclamation = SEFindObjectByName("Esclamation Mark");


					if (esclamation)
					{
						Texture* esclamation_tex = (Texture*)esclamation->getComponent(CT_TEXTURE);

						if (esclamation_tex)
						{
							esclamation_tex->isRendering = false;
						}
					}
					block_move = false;
				}

				else if (go->getName() == "GO Retry1" && m_Collision->isActive)
				{
					GameObject* gameover_go = SEFindObjectByName("Game Over Screen0");
					GameObject* retrybtn_go = SEFindObjectByName("GO Retry1");
					GameObject* exitbtn_go = SEFindObjectByName("GO Exit2");

					if (gameover_go)
					{
						Texture* gameover_text = (Texture*)gameover_go->getComponent(CT_TEXTURE);
						if (gameover_text)
						{
							gameover_text->isRendering = false;
						}
					}

					if (retrybtn_go)
					{
						Texture* retrybtn_text = (Texture*)retrybtn_go->getComponent(CT_TEXTURE);
						retrybtn_text->isRendering = false;
					}

					if (exitbtn_go)
					{
						Texture* exitbtn_text = (Texture*)exitbtn_go->getComponent(CT_TEXTURE);
						exitbtn_text->isRendering = false;
					}

					p_Scene->SetCurrentScene(LEVEL::EXIT);
					button_route_state = ButtonRoute::RESTART_PROMPT; // retry?
				}

				else if (go->getName() == "GO Exit2" && m_Collision->isActive)
				{
					button_route_state = ButtonRoute::MAINMENU_PROMPT;
					p_Scene->SetCurrentScene(LEVEL::EXIT);
				}

				else if (go->getName() == "WIN MM BTN2" && m_Collision->isActive)
				{
					button_route_state = ButtonRoute::MAINMENU_PROMPT;
					p_Scene->SetCurrentScene(LEVEL::MAIN_MENU);
				}

				else if (go->getName() == "HTP Btn Default39_Level_1")
				{
					GameObject* go_button = SEFindObjectByName("HTP Btn Default39_Level_1");
					Texture* tex1 = (Texture*)go_button->getComponent(CT_TEXTURE);
					GameObject* go_HTP = SEFindObjectByName("HTP Bg38_Level_1");
					Texture* tex2 = (Texture*)go_HTP->getComponent(CT_TEXTURE);

					tex1->isRendering = false;
					tex2->isRendering = false;
					block_move = false;
				}
			}

			GameObject* _go = SEFindObjectByName("UI Arrow");
			if (_go)
			{
				Behaviour_Arrow* arrow = (Behaviour_Arrow*)_go->m_behaviour.get();
				if (arrow)
				{
					arrow->m_currentAction = ACTION::ACTION_ACTIVE;
					const auto& current_Transform = m_Transform->getTranslate(); (void)current_Transform;
					arrow->m_Transform->setTranslate({ 0 /*current_Transform.x - 50*/, /*current_Transform.y*/ 0 });
				}
			}
		}
		else if (go->getName() == "Resolution Option")
		{
			if (mode == MODE::WINDOWED)
			{
				m_CurrentAction = ACTION::ACTION_INACTIVE;
				SESetFullScreen(0);
			}
			else if (mode == MODE::FULLSCREEN)
			{
				m_CurrentAction = ACTION::ACTION_ACTIVE;
				SESetFullScreen(1);
			}
		}

		else
		{
			m_CurrentAction = ACTION::ACTION_BUTTON_IDLE;

			GameObject* _go = SEFindObjectByName("UI Arrow");
			if (_go)
			{
				Behaviour_Arrow* arrow = (Behaviour_Arrow*)_go->m_behaviour.get();
				if (arrow)
				{
					arrow->m_currentAction = ACTION::ACTION_INACTIVE;
					// const auto& current_Transform = m_Transform->getTranslate();
					// arrow->m_Transform->setTranslate({ current_Transform.x - 100.0f, current_Transform.y });
				}
			}
		}

	}

	std::string arrow = go->getName().substr(0, 5);
	if (m_CurrentAction == ACTION::ACTION_BUTTON_ISHOVERED && arrow != "Arrow")
	{
		if (m_Texture->AnimationHandler.i > m_Texture->AnimationFrames[m_CurrentAction] - 3)
			m_Texture->AnimationHandler.i = m_Texture->AnimationFrames[m_CurrentAction] - 2;
		if (m_Texture->AnimationHandler.i < 0)
			m_Texture->AnimationHandler.i = 0;
	}
}

void Behaviour_Button::CheckRenderOff()
{
	for (auto go_pair : goArray)
	{
		GameObject* _go = go_pair.second.second;
		Dialogue* dial = (Dialogue*)_go->getComponent(CT_DIALOGUE);
		Texture* texture = (Texture*)_go->getComponent(CT_TEXTURE);

		if (dial)
		{
			if (dial->parent_npc == parent_npc)
			{
				texture->isRendering = false;
			}
		}
	}
}

void Behaviour_NPC::Init()
{
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	// currentWaypoint = m_Transform->getTranslate();
	m_currentAction = ACTION::ACTION_IDLE;
	// nextWayPoint = { m_Transform->getTranslate().x + 251, m_Transform->getTranslate().y };
	m_Type = ITEM_TYPE::NPC;

	current_waypoint_index = 0;

	direction = { 0,0 };

	// Trying to solve npc moving faster everytime we reset
	if (waypoint_init_once == false)
	{
		std::vector<SEVec2> firstSectionWP;
		firstSectionWP.push_back(SEVec2{ -400, -42 });
		firstSectionWP.push_back(SEVec2{ -750, -42 });
		Waypoints.push_back(firstSectionWP);

		std::vector<SEVec2> secondSectionWp;
		secondSectionWp.push_back(SEVec2{ -300, -42 });
		secondSectionWp.push_back(SEVec2{ -300, 300 });
		secondSectionWp.push_back(SEVec2{ 300, 300 });
		secondSectionWp.push_back(SEVec2{ 300, 500 });
		secondSectionWp.push_back(SEVec2{ -300, 500 });
		Waypoints.push_back(secondSectionWp);

		// Lala (lv1) waypoints
		std::vector<SEVec2> lv1firstlalawp;
		lv1firstlalawp.push_back(SEVec2{ -140, -42 });
		lv1firstlalawp.push_back(SEVec2{ -340, -42 });
		std::pair<NPCLalaRoute, std::vector<SEVec2>> lv1_lala_route_1 = std::make_pair(NPCLalaRoute::LV1_ROUTE_1, lv1firstlalawp);
		lala_waypoints.push_back(lv1_lala_route_1);

		// Lala (lv2) waypoints
		std::vector<SEVec2> lv2firstlalawp;
		lv2firstlalawp.push_back(SEVec2{ 200, -42 });
		lv2firstlalawp.push_back(SEVec2{ 600, -42 });
		lv2firstlalawp.push_back(SEVec2{ 600,  150 });
		lv2firstlalawp.push_back(SEVec2{ 600, -42 });
		//lv2firstlalawp.push_back(SEVec2{ 500,  150 });
		std::pair<NPCLalaRoute, std::vector<SEVec2>> lv2_lala_route_1 = std::make_pair(NPCLalaRoute::LV2_ROUTE_1, lv2firstlalawp);
		lala_waypoints.push_back(lv2_lala_route_1);

		std::vector<SEVec2> lv1guardposition;
		lv1guardposition.push_back(SEVec2{ -54, -37 });
		std::pair<NPCMooseRoute, std::vector<SEVec2>> lv1_moose_guard = std::make_pair(NPCMooseRoute::LV1_GUARD, lv1guardposition);
		moose_waypoints.push_back(lv1_moose_guard);

		std::vector<SEVec2> lv2_link;
		lv2_link.push_back(SEVec2{ 223 , -37 });
		std::pair<NPCMooseRoute, std::vector<SEVec2>> lv2_moose_link = std::make_pair(NPCMooseRoute::LV2_LINK, lv2_link);
		moose_waypoints.push_back(lv2_moose_link);

		// Moose (lv2) waypoints 
		std::vector<SEVec2> lv2firstmoosewp;
		lv2firstmoosewp.push_back(SEVec2{ 223 , -240 });
		lv2firstmoosewp.push_back(SEVec2{ 222 , -672 });
		lv2firstmoosewp.push_back(SEVec2{ 479 , -672 });
		lv2firstmoosewp.push_back(SEVec2{ 479 , -240 });
		std::pair<NPCMooseRoute, std::vector<SEVec2>> lv2_moose_route_1 = std::make_pair(NPCMooseRoute::LV2_ROUTE_1, lv2firstmoosewp);
		moose_waypoints.push_back(lv2_moose_route_1);

		waypoint_init_once = true;
	}


	//std::vector<SEVec2> lala_second_waypoint;

	// Get Direction to move towards waypoint
	currentWaypoint = Waypoints[0][0];
	float xWpDist = currentWaypoint.x - m_Transform->getTranslate().x;
	if (xWpDist < 0)
		xWpDist = -xWpDist;
	float yWpDist = currentWaypoint.y - m_Transform->getTranslate().y;
	if (yWpDist < 0)
		yWpDist = -yWpDist;

	m_moveDir = 0; // UP
	if (xWpDist > yWpDist)
	{
		if (currentWaypoint.x < m_Transform->getTranslate().x)
			m_moveDir = 2; // LEFT
		else
			m_moveDir = 3; // RIGHT
	}
	else
	{
		if (currentWaypoint.y > m_Transform->getTranslate().y)
			m_moveDir = 0; // UP
		else
			m_moveDir = 1; // DOWN
	}
	m_prevMoveDir = m_moveDir;
	waypointIndex = 0;
	InitVal.Muse.currentsequence = currentSequence = 0;

	if (go->getName() == "NPC Lala45") {
		InitVal.Lala.Pos = m_Transform->getTranslate();
	}

	else if (go->getName() == "NPC Muse32") {
		InitVal.Muse.Pos = m_Transform->getTranslate();
	}

}



void Behaviour_NPC::Update()
{

	if (!toggle_off)
	{
		SEDialogueToggleOff("NPC Lala45");
		toggle_off = true;
	}
	UpdateNPCFacing();

	//current_level = LEVEL::LEVEL_2;
	assert(m_Collision);
	m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
	auto NPCpos = m_Transform->getTranslate();
	auto NPCScale = m_Transform->getScale();

	float deltaTime = (float)SEGetDeltaTime();



	// Waypoint movement
	if (go->getName() == "NPC Muse32")
	{
		GameObject* proximity = SEFindObjectByName("NPC Muse32 Proximity");
		Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();

		if (proximity_behave->chasing == false && block_move == false)
		{
			// Translation per frame
			/*float displacement = m_movementSpeed * 100 * deltaTime;
			if (displacement > 5)
				displacement = 5;*/

			std::vector<SEVec2> waypointSection = Waypoints[currentSequence];

			// When path open up
			bool newSequenceBegin = false;
			if (SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE) && currentSequence == 0)
			{
				currentSequence++;
				waypointSection = Waypoints[currentSequence];
				waypointIndex = 0;
				currentWaypoint = waypointSection[waypointIndex];
				newSequenceBegin = true;
			}

			// Get distance to waypoint
			float xWpDist = currentWaypoint.x - m_Transform->getTranslate().x;
			if (xWpDist < 0)
				xWpDist = -xWpDist;
			float yWpDist = currentWaypoint.y - m_Transform->getTranslate().y;
			if (yWpDist < 0)
				yWpDist = -yWpDist;
			float distanceSquare = xWpDist * xWpDist + yWpDist * yWpDist;

			if (distanceSquare < 50 || newSequenceBegin)
			{
				if (!newSequenceBegin)
				{
					waypointIndex++;
				}
				if (current_level == LEVEL::LEVEL_2)
				{
					if (waypointIndex != 4 && waypointIndex != 2)
					{
						waypointIndex = 1;
					}
				}

				else
				{
					if (waypointIndex == waypointSection.size())
					{
						waypointIndex = 0;
						if (currentSequence == 1)
							waypointIndex = 1;
					}
				}

				// Get Direction to move towards next waypoint
				currentWaypoint = waypointSection[waypointIndex];
			}

			auto _NPCScale = m_Transform->getScale();

			// Set Proximity scale also.
			GameObject* proximity_go = SEFindObjectByName("NPC Muse32 Proximity");
			Transform* proximity_trans = (Transform*)proximity_go->getComponent(CT_TRANSFORM);
			Collision* proximity_col = (Collision*)proximity_go->getComponent(CT_COLLISION); (void)proximity_col;

			auto proximity_scale = proximity_trans->getScale();

			if (currentWaypoint.x < m_Transform->getTranslate().x)
			{
				/*if (NPCScale.x > 0)
				{
					NPCScale.x = -NPCScale.x;
				}*/
				_NPCScale.x = m_Transform->m_starting_scale.x;
				_NPCScale.y = m_Transform->m_starting_scale.y;
			}
			else
			{
				/*if (NPCScale.x < 0)
				{
					NPCScale.x = -NPCScale.x;
				}*/
				_NPCScale.x = -m_Transform->m_starting_scale.x;
				_NPCScale.y = m_Transform->m_starting_scale.y;
			}
			m_Transform->setScale({ _NPCScale * 1.05f });


			MoveTowardsPosition(currentWaypoint, NPC_MOVEMENT_SPEED);
			//UpdateNPCFacing(); // update the flag so the cone can follow.
			m_currentAction = ACTION::ACTION_WALKING;

			static bool MuseIsGasping = false;
			static float MuseGaspTimer = 1.0;
			GameObject* player_9 = SEFindObjectByName("Player9");
			Behaviour_Player* player_behave = (Behaviour_Player*)player_9->m_behaviour.get();
			// Stop moving to interrogate
			if (CollidePlayer() && !(player_behave->m_currentHoldingItem == Behaviour_Player::INTERACTABLE::NONE))
			{
				m_Transform->setTranslate({ NPCpos.x, NPCpos.y });
				m_Transform->setScale({ _NPCScale });
				m_currentAction = ACTION::ACTION_IDLE;

				if (!MuseIsGasping)
				{
					//SEPlaySound(AUDIOTYPE::SFX, "Deer_Gasp.wav");
					MuseIsGasping = true;
					MuseGaspTimer = 1.0;
				}
			}
			else
			{
				if (MuseGaspTimer > 0)
					MuseGaspTimer -= deltaTime;
				else
					MuseIsGasping = false;
			}
		}
	}

	if (block_move)
	{
		m_currentAction = ACTION::ACTION_IDLE;
		m_Transform->setTranslate(m_Transform->getTranslate());

		auto _NPCScale = m_Transform->getScale();
		if (_NPCScale.x > 0)
		{
			_NPCScale.x = m_Transform->m_starting_scale.x;
			_NPCScale.y = m_Transform->m_starting_scale.y;
		}
		else
		{
			_NPCScale.x = m_Transform->m_starting_scale.x;
			_NPCScale.y = m_Transform->m_starting_scale.y;
		}
		m_Transform->setScale({ _NPCScale });
	}

	if (go->getName() == "NPC Lala45" && block_move == false)
	{
		Transform* pTransform_lala = (Transform*)go->getComponent(CT_TRANSFORM);
		SEVec2 pos;

		// [Level 1] NPC Lala movement
		//if (current_level == LEVEL::LEVEL_1)
		//{
		//	if (SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE))
		//	{
		//		if (pTransform_lala)
		//		{
		//			if (stopFlipping)
		//				pTransform_lala->setScale(-pTransform_lala->getScale().x, pTransform_lala->getScale().y);
		//			stopFlipping = false;

		//			static glm::vec2 fixedPos = { -141.0f, pTransform_lala->getTranslate().y };
		//			pos = pTransform_lala->getTranslate();

		//			for (auto waypoints : lala_waypoints) // iterate through vector to find correct route.
		//			{
		//				if (waypoints.first == NPCLalaRoute::LV1_ROUTE_1)
		//				{
		//					if (pTransform_lala->getTranslate().x < waypoints.second[0].x)
		//					{
		//						pTransform_lala->setTranslate({ pos.x += lala_movement_speed, pTransform_lala->getTranslate().y });
		//						m_currentAction = ACTION::ACTION_WALKING;
		//					}
		//					else
		//						m_currentAction = ACTION::ACTION_IDLE;

		//					if (pTransform_lala->getTranslate().x == waypoints.second[0].x && lala_stops_banging_her_head_back_and_forth == false)
		//					{
		//						pTransform_lala->setScale(-pTransform_lala->getScale().x, pTransform_lala->getScale().y);
		//						lala_stops_banging_her_head_back_and_forth = true;
		//					}
		//				}
		//			}
		//		}
		//	}
		//}

		// [Level 2] NPC Lala movement
		GameObject* proximity = SEFindObjectByName("NPC Lala45 Proximity");
		Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();
		if (proximity_behave->chasing == false)
		{
			if (current_level == LEVEL::LEVEL_1 && SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE))
			{
				if (pTransform_lala && check_move)
				{
					pos = pTransform_lala->getTranslate();

					MoveToWaypoint(NPCLalaRoute::LV1_ROUTE_1, pTransform_lala, true);
				}
			}
			// [Level 2] NPC Lala movement
			else if (current_level == LEVEL::LEVEL_2)
			{
				if (pTransform_lala && check_move)
				{
					pos = pTransform_lala->getTranslate();

					MoveToWaypoint(NPCLalaRoute::LV2_ROUTE_1, pTransform_lala, true);
				}
			}
		}
	}

	if (go->getName() == "NPC Moose" & (block_move == false))
	{
		GameObject* proximity = SEFindObjectByName("NPC Moose Proximity");
		Behaviour_Proximity* proximity_behave = (Behaviour_Proximity*)proximity->m_behaviour.get();
		Transform* pTransform_moose = (Transform*)go->getComponent(CT_TRANSFORM);
		SEVec2 pos;

		if (current_level == LEVEL::LEVEL_1)
		{
			MoveToWaypoint(NPCMooseRoute::LV1_GUARD, pTransform_moose, true);

		}

		if (!move_link && current_level == LEVEL::LEVEL_2)
		{
			if (m_Transform->getTranslate().x < 220)
			{
				MoveToWaypoint(NPCMooseRoute::LV2_LINK, pTransform_moose, true);
			}
			else
			{
				move_link = true;
			}
		}

		if (current_level == LEVEL::LEVEL_2)
		{
			if (proximity_behave->chasing == false)
			{
				if (pTransform_moose && move_link)
				{
					pos = pTransform_moose->getTranslate();

					MoveToWaypoint(NPCMooseRoute::LV2_ROUTE_1, pTransform_moose, true);
				}


			}
		}
	}

}

SEBool Behaviour_NPC::CollidePlayer()
{
	GameObject* go_player = SEFindObjectByName("Player9");
	GameObject* go_npc_muse = SEFindObjectByName("NPC Muse32");

	Collision* player_col = (Collision*)go_player->getComponent(CT_COLLISION);
	Collision* player_npc = (Collision*)go_npc_muse->getComponent(CT_COLLISION);

	if (SECheckCollision(player_col, player_npc))
	{
		return true;
	}

	else
	{
		return false;
	}
}


void Behaviour_NPC::MoveTowardsTarget(GameObject* target, float speed)
{
	if (speed != 0)
	{
		m_movementSpeed = speed;
	}

	//chasing = true; // set flag.

	//SEVec2 direction;
	float distance;
	SEVec2 npc_move_vector;

	if (target)
	{
		static float chasetimer = -1;
		float dt = (float)SEGetDeltaTime() < 0.1f ? (float)SEGetDeltaTime() : 0.1f; (void)dt;

		Transform* target_trans = (Transform*)target->getComponent(CT_TRANSFORM);
		static SEVec2 MuseTargetPos = target_trans->getTranslate();

		chasetimer -= (float)SEGetDeltaTime();
		static std::vector<PathFinding::Node> path;

		// Get square distance between Muse and where it was heading to
		float xTargetDist = MuseTargetPos.x - m_Transform->getTranslate().x;
		float yTargetDist = MuseTargetPos.y - m_Transform->getTranslate().y;
		float distanceSquare = xTargetDist * xTargetDist + yTargetDist * yTargetDist;

		if (distanceSquare < 300)
			chasetimer = -1;

		// If reached waypoint or a certain time since last path get, get new path
		if (chasetimer < 0)
		{
			chasetimer = 0.5f;
			path = PathFinding::WayPointSystem::getPath(m_Transform->getTranslate(), target);

			if (path.size() > 0)
			{
				if (path.size() == 1)
				{
					Transform* WP1Trans = (Transform*)path[0].gameObject->getComponent(CT_TRANSFORM);
					SEVec2 NPCtoWP1 = glm::normalize(WP1Trans->getTranslate() - m_Transform->getTranslate());
					SEVec2 NPCtoPlayer = glm::normalize(target_trans->getTranslate() - m_Transform->getTranslate());
					// Dot product of NPC to WP1 and NPC to Player so it won't parkour over tables
					float dotprod = NPCtoWP1.x * NPCtoPlayer.x + NPCtoWP1.y * NPCtoPlayer.y;
					// Choose between heading towards WP1 or player
					// If WP1 behind NPC relative to Player, head to Player
					if (dotprod > 0.25f)
					{
						MuseTargetPos = WP1Trans->getTranslate();
					}
					else
					{
						MuseTargetPos = target_trans->getTranslate();
					}
				}
				else
				{
					Transform* WP1Trans = (Transform*)path[0].gameObject->getComponent(CT_TRANSFORM);
					Transform* WP2Trans = (Transform*)path[1].gameObject->getComponent(CT_TRANSFORM);
					SEVec2 NPCtoWP1 = glm::normalize(WP1Trans->getTranslate() - m_Transform->getTranslate());
					SEVec2 NPCtoWP2 = glm::normalize(WP2Trans->getTranslate() - m_Transform->getTranslate());
					float dotprod = NPCtoWP1.x * NPCtoWP2.x + NPCtoWP1.y * NPCtoWP2.y;
					// Choose between heading towards WP1 or WP2
					// If WP1 behind NPC relative to WP2, head to WP2
					if (dotprod > 0.05f)
					{
						MuseTargetPos = WP1Trans->getTranslate();
					}
					else
					{
						MuseTargetPos = WP2Trans->getTranslate();
					}
				}
			}
			else
			{
				MuseTargetPos = target_trans->getTranslate();
			}

		}

		if (target_trans && m_Transform)
		{
			// [1] Determine Player's Location
			//SEVec2 playerTranslate = target_trans->getTranslate();
			SEVec2 vector_a_minus_b = MuseTargetPos - m_Transform->getTranslate(); // Vector from [NPC] to [Player]
			distance = (float)vector_a_minus_b.length();

			// [2] Calculates Direction to Move in.
			direction = glm::normalize(vector_a_minus_b); // Normalize Vector  (unit vector)

			// [3] Calculate Distance per frame.
			// distance = m_movementSpeed * SEGetDeltaTime();
			npc_move_vector = direction * m_movementSpeed * (float)SEGetDeltaTime();;// How much + which direction this NPC will be moving

			m_Transform->setTranslate(m_Transform->getTranslate() + npc_move_vector);

			float xWpDist = MuseTargetPos.x - m_Transform->getTranslate().x;
			if (xWpDist < 0)
				xWpDist = -xWpDist;
			float yWpDist = MuseTargetPos.y - m_Transform->getTranslate().y;
			if (yWpDist < 0)
				yWpDist = -yWpDist;

			// Set scale of NPC to the direction it's facing
			auto NPCScale = m_Transform->getScale();
			if (MuseTargetPos.x < m_Transform->getTranslate().x)
			{
				/*if (NPCScale.x > 0)
					NPCScale.x = -NPCScale.x;*/
				NPCScale.x = m_Transform->m_starting_scale.x;
				NPCScale.y = m_Transform->m_starting_scale.y;
			}
			else
			{
				/*if (NPCScale.x < 0)
					NPCScale.x = -NPCScale.x;*/
				NPCScale.x = -m_Transform->m_starting_scale.x;
				NPCScale.y = m_Transform->m_starting_scale.y;
			}
			m_Transform->setScale({ NPCScale * 1.05f });
		}
	}
}

void Behaviour_NPC::MoveTowardsPosition(SEVec2 position, float speed)
{
	if (speed != 0)
	{
		m_movementSpeed = speed;
	}

	float distance;
	SEVec2 npc_move_vector;

	// [1] Determine Player's Location
	SEVec2 vector_a_minus_b = position - m_Transform->getTranslate(); // Vector from [NPC] to [Player]
	distance = (float)vector_a_minus_b.length();

	// [2] Calculates Direction to Move in.
	direction = glm::normalize(vector_a_minus_b); // Normalize Vector  (unit vector)

	// [3] Calculate Distance per frame.
	// distance = m_movementSpeed * SEGetDeltaTime();
	float dt = (float)SEGetDeltaTime() < 0.1f ? (float)SEGetDeltaTime() : 0.1f;
	npc_move_vector = direction * m_movementSpeed * dt; // How much + which direction this NPC will be moving
	SEVec2 predicted_position = position + npc_move_vector;

	m_Transform->setTranslate(m_Transform->getTranslate() + npc_move_vector);
}

void Behaviour_NPC::MoveToWaypoint(NPCLalaRoute route, Transform* npc_transform, NPCFacing final_npc_facing)
{
	SEVec2 waypoint_pos;
	SEVec2 npc_position = npc_transform->getTranslate();
	SEVec2 npc_scale = npc_transform->getScale();

	for (auto waypoints : lala_waypoints) // iterate through vector to find correc route. 
	{
		if (waypoints.first == route)
		{
			// Current Waypoint Position
			waypoint_pos = waypoints.second[current_waypoint_index];

			// Unit Vector (Direction)
			direction = waypoint_pos - npc_position;
			direction = glm::normalize(direction);

			float dt = (float)SEGetDeltaTime() < 0.025f ? (float)SEGetDeltaTime() : 0.025f;
			SEVec2 predicted_position = npc_position + direction * NPC_MOVEMENT_SPEED * dt;

			// [a] Let's settle x-axis first.
			if (direction.x > 0) // walking to direction right 
			{
				if (predicted_position.x < waypoint_pos.x) // [Next Frame] : if npc have yet to reach the designated waypoint (x-axs) 
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else // [Next Frame] : if npc have yet to reach the designated waypoint (x-axis)
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}

				// Check for sprite facing placement (if positive) -> face right
				if (npc_scale.x < 0) // if its negative (facing the left)
					npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face right
			}

			else // walking to direction left
			{
				if (predicted_position.x > waypoint_pos.x)
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}


				// Check for sprite facing placement (if negative) -> face left
				if (npc_scale.x > 0) // if its positive (facing the right)
					npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face left
			}
		}

		if (x_wp_check) // if [current waypoint] has reached. 
		{
			x_wp_check = false; // set it back to false.
			if (current_waypoint_index < waypoints.second.size() - 1)
				current_waypoint_index++; // move to the next index. (if there is still other index)
			else
			{
				switch (final_npc_facing)
				{
				case NPCFacing::LEFT:
					if (npc_transform->getScale().x > 0) // if currently facing right
					{
						npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face left
					}
					break;
				case NPCFacing::RIGHT:
					if (npc_transform->getScale().x < 0) // if currently facing left
					{
						npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face right
					}
					break;
				}
			}
		}
	}
}

void Behaviour_NPC::MoveToWaypoint(NPCMooseRoute moose_route, Transform* npc_transform, bool looping)
{
	SEVec2 waypoint_pos;
	SEVec2 npc_position = npc_transform->getTranslate();
	SEVec2 npc_scale = npc_transform->getScale();

	for (auto waypoints : moose_waypoints) // iterate through vector to find correc route. 
	{
		if (waypoints.first == moose_route)
		{
			// Current Waypoint Position
			waypoint_pos = waypoints.second[current_waypoint_index];

			// Unit Vector (Direction)
			direction = waypoint_pos - npc_position;
			direction = glm::normalize(direction);

			float dt = (float)SEGetDeltaTime() < 0.025f ? (float)SEGetDeltaTime() : 0.025f;
			SEVec2 predicted_position = npc_position + direction * NPC_MOVEMENT_SPEED * dt;

			// [a] Let's settle x-axis first.
			if (direction.x > 0) // walking to direction right 
			{
				if (predicted_position.x < waypoint_pos.x) // [Next Frame] : if npc have yet to reach the designated waypoint (x-axs) 
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else // [Next Frame] : if npc have yet to reach the designated waypoint (x-axis)
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}

				// Check for sprite facing placement (if positive) -> face right
				//if (npc_scale.x < 0) // if its negative (facing the left)
				//	npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face right

				auto NPCScale = npc_transform->getScale();
				NPCScale.x = -npc_transform->m_starting_scale.x;
				NPCScale.y = npc_transform->m_starting_scale.y;
				npc_transform->setScale({ NPCScale * 1.05f });
			}

			else if (direction.x < 0)// walking to direction left
			{
				if (predicted_position.x > waypoint_pos.x)
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}


				// Check for sprite facing placement (if negative) -> face left
				//if (npc_scale.x > 0) // if its positive (facing the right)
				//	npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face left

				auto NPCScale = npc_transform->getScale();
				NPCScale.x = npc_transform->m_starting_scale.x;
				NPCScale.y = npc_transform->m_starting_scale.y;
				npc_transform->setScale({ NPCScale * 1.05f });
			}
			else if (direction.y > 0) // Up
			{
				if (predicted_position.y < waypoint_pos.y)
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}
			}
			else if (direction.y < 0) // Down
			{
				if (predicted_position.y > waypoint_pos.y)
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}
			}
		}

		if (x_wp_check) // if [current waypoint] has reached. 
		{
			x_wp_check = false; // set it back to false.
			if (current_waypoint_index < waypoints.second.size() - 1)
			{
				current_waypoint_index++; // move to the next index. (if there is still other index)
			}
			else
			{
				if (looping)
				{
					current_waypoint_index = 0;
				}
			}


		}
	}
}

void Behaviour_NPC::MoveToWaypoint(NPCLalaRoute lala_route, Transform* npc_transform, bool looping)
{
	SEVec2 waypoint_pos;
	SEVec2 npc_position = npc_transform->getTranslate();
	SEVec2 npc_scale = npc_transform->getScale();

	for (auto waypoints : lala_waypoints) // iterate through vector to find correc route. 
	{
		if (waypoints.first == lala_route)
		{
			// Current Waypoint Position
			waypoint_pos = waypoints.second[current_waypoint_index];

			// Unit Vector (Direction)
			direction = waypoint_pos - npc_position;
			direction = glm::normalize(direction);

			float dt = (float)SEGetDeltaTime() < 0.025f ? (float)SEGetDeltaTime() : 0.025f;
			SEVec2 predicted_position = npc_position + direction * NPC_MOVEMENT_SPEED * dt;

			// [a] Let's settle x-axis first.
			if (direction.x > 0) // walking to direction right 
			{
				if (predicted_position.x < waypoint_pos.x) // [Next Frame] : if npc have yet to reach the designated waypoint (x-axs) 
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else // [Next Frame] : if npc have yet to reach the designated waypoint (x-axis)
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}

				// Check for sprite facing placement (if positive) -> face right
				//if (npc_scale.x < 0) // if its negative (facing the left)
				//	npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face right

				auto NPCScale = npc_transform->getScale();
				NPCScale.x = -npc_transform->m_starting_scale.x;
				NPCScale.y = npc_transform->m_starting_scale.y;
				npc_transform->setScale({ NPCScale * 1.05f });
			}

			else if (direction.x < 0)// walking to direction left
			{
				if (predicted_position.x > waypoint_pos.x)
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}


				// Check for sprite facing placement (if negative) -> face left
				//if (npc_scale.x > 0) // if its positive (facing the right)
				//	npc_transform->setScale(-npc_transform->getScale().x, npc_transform->getScale().y); // change it to face left

				auto NPCScale = npc_transform->getScale();
				NPCScale.x = npc_transform->m_starting_scale.x;
				NPCScale.y = npc_transform->m_starting_scale.y;
				npc_transform->setScale({ NPCScale * 1.05f });
			}
			else if (direction.y > 0) // Up
			{
				if (predicted_position.y < waypoint_pos.y)
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}
			}
			else if (direction.y < 0) // Down
			{
				if (predicted_position.y > waypoint_pos.y)
				{
					MoveTowardsPosition(waypoint_pos, NPC_MOVEMENT_SPEED); // let npc continue moving
					m_currentAction = ACTION::ACTION_WALKING;
				}

				else
				{
					m_currentAction = ACTION::ACTION_IDLE;
					x_wp_check = true;
				}
			}
		}

		if (x_wp_check) // if [current waypoint] has reached. 
		{
			x_wp_check = false; // set it back to false.
			if (current_waypoint_index < waypoints.second.size() - 1)
			{
				current_waypoint_index++; // move to the next index. (if there is still other index)
			}
			else
			{
				if (looping)
				{
					current_waypoint_index = 0;
				}
			}
		}
	}
}

void Behaviour_NPC::UpdateNPCFacing()
{
	buffer_prev_npc_facing = prev_npc_facing;

	if (direction.x > 0.1) // npc currently facing right
	{
		prev_npc_facing = npc_facing;
		npc_facing = NPCFacing::RIGHT;
		return;
	}

	if (direction.x < -0.1)
	{
		prev_npc_facing = npc_facing;
		npc_facing = NPCFacing::LEFT;
		return;
	}


	if (direction.y < -0.1)
	{
		prev_npc_facing = npc_facing;
		npc_facing = NPCFacing::DOWN;
		return;
	}

	if (direction.y > 0.1)
	{
		prev_npc_facing = npc_facing;
		npc_facing = NPCFacing::UP;
		return;
	}

	// Temp fix - for lala
	if (direction.x == 0)
	{
		npc_facing = NPCFacing::START;
		return;
	}
}

void Behaviour_Evidence::Init()
{
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	m_Type = ITEM_TYPE::EVIDENCE;


	// m_currentAction = ACTION::ACTION_ACTIVE_REAL;

	if (go->getName() == "Paper Real1")
		InitVal.PaperReal.Occupant_Item = PR1_currentOccuItem = Behaviour_Player::INTERACTABLE::PAPER_REAL;
	else if (go->getName() == "Glass Pieces26")
		InitVal.GlassPieces.Occupant_Item = GP26_currentOccuItem = Behaviour_Player::INTERACTABLE::GLASSPIECES;
	else if (go->getName() == "Photo Frame25")
		InitVal.PhotoFrame.Occupant_Item = PF25_currentOccuItem = Behaviour_Player::INTERACTABLE::PHOTO;
	else if (go->getName() == "Broom9")
		InitVal.Broom.Occupant_Item = B9_currentOccuItem = Behaviour_Player::INTERACTABLE::BROOM_PICKUP;
	else if (go->getName() == "Sink21")
		S21_currentOccuItem = Behaviour_Player::INTERACTABLE::NONE;
	else if (go->getName() == "mop0")
		M0_currentOccuItem = Behaviour_Player::INTERACTABLE::MOP_PICKUP;
	else if (go->getName() == "pinkblood3")
		m_currentAction = ACTION::ACTION_ACTIVE;
	else if (go->getName() == "Paint Bucket")
		PB_currentOccuItem = Behaviour_Player::INTERACTABLE::BUCKET_FAKE_BLOOD_FULL;
	else if (go->getName() == "Blood Knife")
		BK_currentOccuItem = Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED;
	else if (go->getName() == "Candy Horn")
		CH_currentOccuItem = Behaviour_Player::INTERACTABLE::REAL_CANDY_HORN;

	else
		m_currentAction = ACTION::ACTION_NULL;
}

void Behaviour_Evidence::Update()
{
	if (current_level == LEVEL::LEVEL_2)
	{
		if (go->getName() == "mop0" && evidence_taken == false)
			M0_currentOccuItem = Behaviour_Player::INTERACTABLE::MOP_PICKUP;
	}

	//if (m_Collision)
	//	m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });


	if (go->getName() == "mop0")
	{
		switch (M0_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_NULL;
			break;
		case Behaviour_Player::INTERACTABLE::INACTIVE:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::MOP_PICKUP:
			m_currentAction = ACTION::ACTION_GLOW;
			break;
		case Behaviour_Player::INTERACTABLE::MOP_PUTDOWN:
			m_currentAction = ACTION::ACTION_ACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::PAPER_REAL:
		case Behaviour_Player::INTERACTABLE::PAPER_FAKE:
		case Behaviour_Player::INTERACTABLE::PHOTO:
		case Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE:
		case Behaviour_Player::INTERACTABLE::BROOM_PICKUP:
		case Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::GLASSPIECES:
		case Behaviour_Player::INTERACTABLE::TRASHBAG:
		case Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED:
		case Behaviour_Player::INTERACTABLE::KNIFE_WASHED:
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}


	}



	if (go->getName() == "Glass Pieces26")
	{
		switch (GP26_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::GLASSPIECES:
			m_currentAction = ACTION::ACTION_ACTIVE_GLASS;
			break;
		case Behaviour_Player::INTERACTABLE::TRASHBAG:
			m_currentAction = ACTION::ACTION_ACTIVE_TRASHBAG;
			break;
		case Behaviour_Player::INTERACTABLE::PAPER_REAL:
		case Behaviour_Player::INTERACTABLE::PAPER_FAKE:
		case Behaviour_Player::INTERACTABLE::PHOTO:
		case Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE:
		case Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::BROOM_PICKUP:
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}

	if (go->getName() == "Photo Frame25")
	{
		switch (PF25_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::PHOTO:
			m_currentAction = ACTION::ACTION_ACTIVE_REAL;
			break;
		case Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE:
			m_currentAction = ACTION::ACTION_ACTIVE_FAKE;
			break;

		case Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::BROOM_PICKUP:
		case Behaviour_Player::INTERACTABLE::GLASSPIECES:
		case Behaviour_Player::INTERACTABLE::TRASHBAG:
		case Behaviour_Player::INTERACTABLE::PAPER_REAL:
		case Behaviour_Player::INTERACTABLE::PAPER_FAKE:
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}

	if (go->getName() == "Paper Real1")
	{
		switch (PR1_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::PAPER_REAL:
			m_currentAction = ACTION::ACTION_ACTIVE_REAL;
			break;
		case Behaviour_Player::INTERACTABLE::PAPER_FAKE:
			m_currentAction = ACTION::ACTION_ACTIVE_FAKE;
			break;
		case Behaviour_Player::INTERACTABLE::PHOTO:
		case Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE:
		case Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::BROOM_PICKUP:
		case Behaviour_Player::INTERACTABLE::TRASHBAG:
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}

	if (go->getName() == "Broom9")
	{
		switch (B9_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::BROOM_PICKUP:
			m_currentAction = ACTION::ACTION_GLOW;
			break;
		case Behaviour_Player::INTERACTABLE::PAPER_REAL:
		case Behaviour_Player::INTERACTABLE::PAPER_FAKE:
		case Behaviour_Player::INTERACTABLE::PHOTO:
		case Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE:
		case Behaviour_Player::INTERACTABLE::GLASSPIECES:
		case Behaviour_Player::INTERACTABLE::TRASHBAG:
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}

	if (go->getName() == "Sink21") {
		switch (S21_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED:
			m_currentAction = ACTION::ACTION_ACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::KNIFE_WASHED:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::SINK_OFF:
			m_currentAction = ACTION::ACTION_INACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::SINK_ON:
			m_currentAction = ACTION::ACTION_ACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::PAPER_REAL:
		case Behaviour_Player::INTERACTABLE::PAPER_FAKE:
		case Behaviour_Player::INTERACTABLE::PHOTO:
		case Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE:
		case Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::BROOM_PICKUP:
		case Behaviour_Player::INTERACTABLE::MOP_PICKUP:
		case Behaviour_Player::INTERACTABLE::MOP_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::MOP_BLOODY:
			m_currentAction = ACTION::ACTION_ACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::GLASSPIECES:
		case Behaviour_Player::INTERACTABLE::TRASHBAG:
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}

	if (go->getName() == "Paint Bucket")
	{
		switch (PB_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_NULL;
			break;
		case Behaviour_Player::INTERACTABLE::BUCKET_FAKE_BLOOD_FULL:
			m_currentAction = ACTION::ACTION_BUCKET_FULL;
			break;
		case Behaviour_Player::INTERACTABLE::BUCKET_EMPTY:
			m_currentAction = ACTION::ACTION_BUCKET_EMPTY;
			break;
		case Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED:
		case Behaviour_Player::INTERACTABLE::SINK_OFF:
		case Behaviour_Player::INTERACTABLE::SINK_ON:
		case Behaviour_Player::INTERACTABLE::PAPER_REAL:
		case Behaviour_Player::INTERACTABLE::PAPER_FAKE:
		case Behaviour_Player::INTERACTABLE::PHOTO:
		case Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE:
		case Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::BROOM_PICKUP:
		case Behaviour_Player::INTERACTABLE::MOP_PICKUP:
		case Behaviour_Player::INTERACTABLE::MOP_PUTDOWN:
		case Behaviour_Player::INTERACTABLE::MOP_BLOODY:
		case Behaviour_Player::INTERACTABLE::GLASSPIECES:
		case Behaviour_Player::INTERACTABLE::TRASHBAG:
		case Behaviour_Player::INTERACTABLE::KNIFE_WASHED:
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}

	if (go->getName() == "Blood Knife")
	{
		switch (BK_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_NULL;
			break;
		case Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED:
			m_currentAction = ACTION::ACTION_ACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::KNIFE_WASHED:
			m_currentAction = ACTION::ACTION_CLEANED_KNIFE;
			break;
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}


	if (go->getName() == "Candy Horn")
	{
		switch (CH_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_NULL;
			break;
		case Behaviour_Player::INTERACTABLE::REAL_CANDY_HORN:
			m_currentAction = ACTION::ACTION_REAL_HORN;
			break;
		case Behaviour_Player::INTERACTABLE::FAKE_CANDY_HORN:
			m_currentAction = ACTION::ACTION_FAKE_HORN;
			break;
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}

	if (go->getName() == "candyPile2")
	{
		switch (CP_currentOccuItem)
		{
		case Behaviour_Player::INTERACTABLE::NONE:
			m_currentAction = ACTION::ACTION_ACTIVE;
			break;
		case Behaviour_Player::INTERACTABLE::CANDY_PILE_SEARCHED:
			m_currentAction = ACTION::ACTION_NULL;
			break;
		default:
#if _DEBUG
			std::cout << "ILLEGAL CASE!" << std::endl;
#endif
			break;
		}
	}
}

/*for (const auto& obj : goArray)
{
	GameObject* gameObj = obj.second.second;
	if (gameObj)
	{
		std::string name = gameObj->getName();

		for (const auto& obj2 : goArray)
		{
			GameObject* gameObj2 = obj2.second.second;
			if (gameObj2)
			{
				std::string name2 = gameObj2->getName();
				if (name == "Paper Real1" && name2 == "Player9")
				{
					Collision* collsion = (Collision*)gameObj->getComponent(CT_COLLISION);
					Collision* collsion2 = (Collision*)gameObj2->getComponent(CT_COLLISION);

					if (collsion && collsion2)
					{
						if (collsion->CollisionCheck(collsion2))
						{

						}
					}
				}
			}
		}
	}
}*/

// Player with Evidence
/*for (const auto& obj : goArray)
{
	GameObject* gameobject = obj.second.second;
	if (gameobject)
	{
		if (gameobject->getName() != go->getName())
		{
			Collision* player_collision = (Collision*)gameobject->getComponent(CT_COLLISION);
			Behaviour_Player* player = (Behaviour_Player*)gameobject->m_behaviour.get();\
			if (player && player_collision)
			{
				if (player_collision->CollisionCheck(m_Collision))
				{
					std::cout << gameobject->getName() << " with " << go->getName() << std::endl;
				}
				else
					std::cout << "no hit with ebide\n";
			}
		}

	}
}*/

void Behaviour_Trigger_Meter::Init()
{
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	InitVal.LogicData.SusLevel = suspicion_level = 0;


}

void Behaviour_Trigger_Meter::Update()
{
	SEVec2 sus_meter_translation;

	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;

		Dialogue* dialogue = (Dialogue*)gameObj->getComponent(CT_DIALOGUE); (void)dialogue;

		// if (dialogue)
		//{
		//	if (dialogue->suspicious_flag && dialogue->button_pressed == true) // if this dialogue component holds a "suspicious" flag.
		//	{
		//		SEDialogueToggleOff();
		//		std::cout << "THIS BUTTON HAS : " << dialogue->suspicion_value << std::endl;
		//		dialogue->button_pressed = true;
		//		SusRotateArrow(); // if this pressed, rotate arrow. (for now value doesn't affect anything.)
		//	}
		// }
	}

	// Respective to Sus Meter
	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;

		Transform* trans = (Transform*)gameObj->getComponent(CT_TRANSFORM);

		if (gameObj->getName() == "Sus Meter Animation")
		{
			sus_meter_translation = trans->getTranslate();
		}
	}


	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;

		Dialogue* dialogue = (Dialogue*)gameObj->getComponent(CT_DIALOGUE); (void)dialogue;
		Transform* trans = (Transform*)gameObj->getComponent(CT_TRANSFORM); (void)trans;

		if (gameObj->getName() == "Sus Arrow")
		{
		}
	}


}

void Behaviour_Trigger_Meter::SusRotateArrow()
{
	SEVec2 sus_meter_translation;
	SEUint sus_value = 0;

	// Respective to Sus Meter
	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;

		Transform* trans = (Transform*)gameObj->getComponent(CT_TRANSFORM);

		if (gameObj->getName() == "Sus Meter Animation")
		{
			sus_meter_translation = trans->getTranslate();
		}
	}

	// Find Dialogue - Suspicion Value
	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;

		Dialogue* dialogue = (Dialogue*)gameObj->getComponent(CT_DIALOGUE);

		if (dialogue && gameObj->getName() == "Secure Bag Option's Boarder")
		{
			sus_value = dialogue->suspicion_value;
		}
	}

	// Rotate Arrow
	for (const auto& obj : goArray)
	{
		GameObject* gameObj = obj.second.second;

		Transform* trans = (Transform*)gameObj->getComponent(CT_TRANSFORM);

		if (gameObj->getName() == "Sus Meter Arrow")
		{
			switch (sus_value)
			{
			case 0:
				trans->setRotationAngle(0.0f);
				break;
			case 1:
				trans->setRotationAngle(42.0f);
				m_glow = ACTION::ACTION_GLOW;
				break;
			case 2:
				trans->setRotationAngle(68.0f);
				m_glow = ACTION::ACTION_GLOW;
				break;
			case 3:
				trans->setRotationAngle(102.0f);
				m_glow = ACTION::ACTION_GLOW;
				break;
			case 4:
				trans->setRotationAngle(138.0f);
				m_glow = ACTION::ACTION_GLOW;
				break;
			case 5:
				trans->setRotationAngle(180.0f);
				m_glow = ACTION::ACTION_GLOW;
				break;
			}
		}
	}
}

void Behaviour_Proximity::Init()
{
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);

	if (go->getName() == "Esclamation Mark")
	{

	}

	proximity_touched = false;
	chasing = false;
	wasChasing = false;
	alert_done = false;
	make_shift_alert_timer = 0;
	make_shift_esclamation_timer = 0;
}

void Behaviour_Proximity::Update()
{
	// Update Proximity to follow NPC.
	GameObject* npc_go = SEFindObjectByName(npc_name);
	Transform* npc_trans = (Transform*)npc_go->getComponent(CT_TRANSFORM);
	Behaviour_NPC* npc_behave = (Behaviour_NPC*)npc_go->m_behaviour.get();

	// Update [NPC's Vision Cone] 
	TranslateVisionCone();

	//if (npc_trans)
	//{
	//	m_Transform->setTranslate(npc_trans->getTranslate());
	//	m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
	//}

	// Check if Player is colliding with this proximity.
	GameObject* player_go = SEFindObjectByName("Player9");
	Collision* player_col = (Collision*)player_go->getComponent(CT_COLLISION);
	Behaviour_Player* player_behave = (Behaviour_Player*)player_go->m_behaviour.get();

	Behaviour_Player::INTERACTABLE player_holding = player_behave->m_currentHoldingItem;

	// Esclamation Mark
	GameObject* esclamation_go = nullptr;
	Texture* esclamation_tex = nullptr;
	Behaviour_Esclamation* esclamation_behave = nullptr;
	if (npc_go->getName() == "NPC Muse32")
	{
		esclamation_go = SEFindObjectByName("Esclamation Mark");
		esclamation_tex = (Texture*)esclamation_go->getComponent(CT_TEXTURE);
		esclamation_behave = (Behaviour_Esclamation*)esclamation_go->m_behaviour.get();
	}
	else if (npc_go->getName() == "NPC Moose")
	{
		esclamation_go = SEFindObjectByName("Esclamation Mark - Akai");
		esclamation_tex = (Texture*)esclamation_go->getComponent(CT_TEXTURE);
		esclamation_behave = (Behaviour_Esclamation*)esclamation_go->m_behaviour.get();
	}
	else if (npc_go->getName() == "NPC Lala45")
	{
		esclamation_go = SEFindObjectByName("Esclamation Mark - Lala45");
		esclamation_tex = (Texture*)esclamation_go->getComponent(CT_TEXTURE);
		esclamation_behave = (Behaviour_Esclamation*)esclamation_go->m_behaviour.get();
	}


	if (player_col)
	{
		if (SECheckCollision(player_col, m_Collision) && player_holding != Behaviour_Player::INTERACTABLE::NONE)
		{
			// [NPC Lala - Chase Logic]
			if (npc_name == "NPC Lala45")
			{
				// Level 1 stuff 
				// ---------------------------------
				if (SEGetChaseLalaRecord(ChaseLalaRecords_LV1::CHASE_BROOM) == false && player_holding == Behaviour_Player::INTERACTABLE::BROOM_PICKUP) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetDialogueLalaStatus(DialogueLalaCheck::NODE_14) == false && player_holding == Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseLalaRecord(ChaseLalaRecords_LV1::CHASE_RUBBISH_BAG) == false && player_holding == Behaviour_Player::INTERACTABLE::TRASHBAG) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseLalaRecord(ChaseLalaRecords_LV1::CHASE_REAL_PHOTO) == false && player_holding == Behaviour_Player::INTERACTABLE::PHOTO) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseLalaRecord(ChaseLalaRecords_LV1::CHASE_FAKE_PHOTO) == false && player_holding == Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE) // haven't chase yet.
				{
					proximity_touched = true;
				}

				// Level 2 stuff
				// ---------------------------------
				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_CLEAN_MOP) == false && player_holding == Behaviour_Player::INTERACTABLE::MOP_PICKUP) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_BLOODY_MOP) == false && player_holding == Behaviour_Player::INTERACTABLE::MOP_BLOODY) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_PAINT_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_FAKE_BLOOD_FULL) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_EMPTY_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_EMPTY) // haven't chase yet.
				{
					proximity_touched = true;
				}


				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_BLOODY_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_CLEAN_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_WASHED) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_CANDY_HORN) == false && player_holding == Behaviour_Player::INTERACTABLE::REAL_CANDY_HORN) // haven't chase yet.
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseLalaRecords_LV2::CHASE_CANDY_CORN) == false && player_holding == Behaviour_Player::INTERACTABLE::FAKE_CANDY_HORN) // haven't chase yet.
				{
					proximity_touched = true;
				}



			}

			// [NPC Muse - Chase Logic]
			if (npc_name == "NPC Muse32")
			{
				// Level 1 stuff 
				// ---------------------------------
				if (SEGetChaseRecord(ChaseRecords_LV1::CHASE_REAL_PAPER) == false && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_1) && player_holding == Behaviour_Player::INTERACTABLE::PAPER_REAL) // haven't chase yet.
				{
					proximity_touched = true;
				}

				else if (SEGetChaseRecord(ChaseRecords_LV1::CHASE_FAKE_PAPER) == false && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_1) && player_holding == Behaviour_Player::INTERACTABLE::PAPER_FAKE) // haven't chase yet.
				{
					proximity_touched = true;
				}

				else if (SEGetChaseRecord(ChaseRecords_LV1::CHASE_BROOM) == false && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_9) && player_holding == Behaviour_Player::INTERACTABLE::BROOM_PICKUP)
				{
					proximity_touched = true;
				}

				// Only this checks for dialogue check 
				else if (SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_9) == false && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_9) && player_holding == Behaviour_Player::INTERACTABLE::BROOM_PUTDOWN)
				{
					proximity_touched = true;
				}

				else if (SEGetChaseRecord(ChaseRecords_LV1::CHASE_RUBBISH_BAG) == false && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_10) && player_holding == Behaviour_Player::INTERACTABLE::TRASHBAG)
				{
					proximity_touched = true;
				}

				else if (SEGetChaseRecord(ChaseRecords_LV1::CHASE_REAL_PHOTO) == false && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_11) && player_holding == Behaviour_Player::INTERACTABLE::PHOTO)
				{
					proximity_touched = true;
				}

				else if (SEGetChaseRecord(ChaseRecords_LV1::CHASE_FAKE_PHOTO) == false && !SEGetDialogueMuseStatus(DialogueMuseCheck::NODE_12) && player_holding == Behaviour_Player::INTERACTABLE::PHOTO_FROM_PILE)
				{
					proximity_touched = true;
				}

				// Level 2 stuff 
				// ---------------------------------
				// Tasks #1
				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_MOP) == false && player_holding == Behaviour_Player::INTERACTABLE::MOP_PICKUP)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_BLOODY_MOP) == false && player_holding == Behaviour_Player::INTERACTABLE::MOP_BLOODY)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_FILLED_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_FAKE_BLOOD_FULL)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_EMPTY_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_EMPTY)
				{
					proximity_touched = true;
				}

				// Task #2
				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_BLOODY_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_WASHED)
				{
					proximity_touched = true;
				}

				// Task #3 
				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CANDY_HORN) == false && player_holding == Behaviour_Player::INTERACTABLE::REAL_CANDY_HORN)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CANDY_CORN) == false && player_holding == Behaviour_Player::INTERACTABLE::FAKE_CANDY_HORN)
				{
					proximity_touched = true;
				}

			}

			if (npc_name == "NPC Moose")
			{
				// Task #1
				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_CLEAN_MOP) == false && player_holding == Behaviour_Player::INTERACTABLE::MOP_PICKUP)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_BLOODY_MOP) == false && player_holding == Behaviour_Player::INTERACTABLE::MOP_BLOODY)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_FILLED_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_FAKE_BLOOD_FULL)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_EMPTY_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_EMPTY)
				{
					proximity_touched = true;
				}

				// Task #2
				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_BLOODY_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_CLEAN_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_WASHED)
				{
					proximity_touched = true;
				}

				// Task #3 
				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_CANDY_HORN) == false && player_holding == Behaviour_Player::INTERACTABLE::REAL_CANDY_HORN)
				{
					proximity_touched = true;
				}

				if (SEGetChaseRecord(ChaseAkaiRecords_LV2::CHASE_CANDY_CORN) == false && player_holding == Behaviour_Player::INTERACTABLE::FAKE_CANDY_HORN)
				{
					proximity_touched = true;
				}


			}


			//else if (SEGetChaseRecord(ChaseRecords_LV2::CHASE_BLOODY_MOP) == false && player_holding == Behaviour_Player::INTERACTABLE::MOP_PUTDOWN)
			/*{
				proximity_touched = true;
			}*/

			//else if (SEGetChaseRecord(ChaseRecords_LV2::CHASE_FILLED_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_FAKE_BLOOD_FULL)
			//{
			//	proximity_touched = true;
			//}

			//else if (SEGetChaseRecord(ChaseRecords_LV2::CHASE_EMPTY_BUCKET) == false && player_holding == Behaviour_Player::INTERACTABLE::BUCKET_EMPTY)
			//{
			//	proximity_touched = true;
			//}

			//else if (SEGetChaseRecord(ChaseRecords_LV2::CHASE_BLOODY_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_UNWASHED)
			//{
			//	proximity_touched = true;
			//}

			//else if (SEGetChaseRecord(ChaseRecords_LV2::CHASE_CLEAN_KNIFE) == false && player_holding == Behaviour_Player::INTERACTABLE::KNIFE_WASHED)
			//{
			//	proximity_touched = true;
			//}

			//else if (SEGetChaseRecord(ChaseRecords_LV2::CHASE_CANDY_CORN) == false && player_holding == Behaviour_Player::INTERACTABLE::FAKE_CANDY_HORN)
			//{
			//	proximity_touched = true;
			//}

			//else if (SEGetChaseRecord(ChaseRecords_LV2::CHASE_CANDY_HORN) == false && player_holding == Behaviour_Player::INTERACTABLE::REAL_CANDY_HORN)
			//{
			//	proximity_touched = true;
			//}
		}

		if (npc_go->getName() == "NPC Lala45" && !SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE))
		{
			proximity_touched = false;
			chasing = false;
		}

		// Level 2
		if (npc_go->getName() == "NPC Muse32" && SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_MOP))
		{
			proximity_touched = false;
			chasing = false;
		}

		if (npc_go->getName() == "NPC Muse32" && SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_MOP))
		{
			proximity_touched = false;
			chasing = false;
		}

		if (npc_go->getName() == "NPC Muse32" && SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_MOP))
		{
			proximity_touched = false;
			chasing = false;
		}

		if (npc_go->getName() == "NPC Muse32" && SEGetChaseRecord(ChaseMuseRecords_LV2::CHASE_CLEAN_MOP))
		{
			proximity_touched = false;
			chasing = false;
		}

		if (proximity_touched)
		{
			float dt = (float)SEGetDeltaTime() < 0.1f ? (float)SEGetDeltaTime() : 0.1f;
			make_shift_alert_timer += dt;
			make_shift_esclamation_timer += dt;

			if (wasChasing == false)
			{
				if (npc_go->getName() == "NPC Muse32")
					SEPlaySound(AUDIOTYPE::SFX, "Deer_Gasp.wav");
				else if (npc_go->getName() == "NPC Lala45")
					SEPlaySound(AUDIOTYPE::SFX, "Rabbit_Gasp.wav");
			}

			wasChasing = true;

			//#if _DEBUG
			//			std::cout << make_shift_alert_timer << std::endl;
			//#endif

			// NPC Logic - Alert + Chase
			if (make_shift_alert_timer < 1.0f)
			{
				npc_behave->m_currentAction = ACTION::ACTION_ALERT; // Change animation to alert animation
				npc_trans->setTranslate(npc_trans->getTranslate()); // stop him in places
				chasing = true;
			}

			else if (block_move == false) // keep chasing until collision.
			{
				npc_behave->m_currentAction = ACTION::ACTION_WALKING;
				npc_behave->MoveTowardsTarget(player_go, NPC_MOVEMENT_SPEED * 1.5f);

				Transform* playerPos = (Transform*)player_go->getComponent(CT_TRANSFORM);
				float xTargetDist = playerPos->getTranslate().x - npc_trans->getTranslate().x;
				float yTargetDist = playerPos->getTranslate().y - npc_trans->getTranslate().y;
				float distanceSquare = xTargetDist * xTargetDist + yTargetDist * yTargetDist;
				if (distanceSquare > 125000)
				{
					chasing = false;
					proximity_touched = false;
				}

				if (!chasing)
					proximity_touched = false;
			}

			if (make_shift_esclamation_timer < 0.5f)
			{
				esclamation_tex->isRendering = true;
				esclamation_behave->m_currentAction = ACTION::ACTION_EXCLAMATION;

			}

			else
			{
				esclamation_behave->m_currentAction = ACTION::ACTION_ACTIVE;
			}



		}

		// Assuming this code makes the [NPC] go back to their designated waypoint when not chasing anymore.
		else if (wasChasing && !chasing)
		{
			esclamation_tex->isRendering = false;
			// change current waypoint to nearest waypoint
			wasChasing = false;
			std::vector<SEVec2> waypointSection;
			if (npc_go->getName() == "NPC Muse32")
				waypointSection = npc_behave->Waypoints[npc_behave->currentSequence];
			else if (npc_go->getName() == "NPC Moose")
			{
				for (auto waypoints : npc_behave->moose_waypoints) // iterate through vector to find correc route. 
				{
					if (waypoints.first == NPCMooseRoute::LV2_ROUTE_1)
					{
						waypointSection = waypoints.second;
					}
				}
			}
			else if (npc_go->getName() == "NPC Lala45")
			{
				for (auto waypoints : npc_behave->lala_waypoints) // iterate through vector to find correc route. 
				{
					if (current_level == LEVEL::LEVEL_1 && SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE))
					{
						if (waypoints.first == NPCLalaRoute::LV1_ROUTE_1)
						{
							waypointSection = waypoints.second;
						}
					}
					// [Level 2] NPC Lala movement
					else if (current_level == LEVEL::LEVEL_2)
					{
						if (waypoints.first == NPCLalaRoute::LV2_ROUTE_1)
						{
							waypointSection = waypoints.second;
						}
					}
				}
			}

			int nearestIndex = 0;
			for (int i = 1; i < waypointSection.size(); i++)
			{
				SEVec2 nearestVec = npc_trans->getTranslate() - waypointSection[nearestIndex];
				float nearestSqrDistance = nearestVec.x * nearestVec.x + nearestVec.y * nearestVec.y;

				SEVec2 newVec = npc_trans->getTranslate() - waypointSection[i];
				float newSqrDistance = newVec.x * newVec.x + newVec.y * newVec.y;

				if (newSqrDistance < nearestSqrDistance)
					nearestIndex = i;
			}

			npc_behave->waypointIndex = nearestIndex;
			npc_behave->currentWaypoint = waypointSection[nearestIndex];
			npc_behave->current_waypoint_index = nearestIndex;

		}

		else
		{
			make_shift_alert_timer = 0;
			make_shift_esclamation_timer = 0;
		}

	}
}

void Behaviour_Proximity::TranslateVisionCone()
{

	GameObject* npc_go = SEFindObjectByName(npc_name);

	SEVec2 npc_translate{};

	if (npc_go)
	{
		Transform* npc_transform = (Transform*)npc_go->getComponent(CT_TRANSFORM);
		Behaviour_NPC* npc_behave = dynamic_cast<Behaviour_NPC*>(npc_go->m_behaviour.get());

		if (npc_transform)
		{
			npc_translate = npc_transform->getTranslate();
		}

		// Translate the [Proximity Cone] in front of the [NPC]
		//m_Transform->setTranslate(SEVec2{ npc_translate.x + 100, npc_translate.y });
		//// Remember to update the collision box too!
		//m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });

		// !! Remember to flip the cone with regards on where the npc is facing
		if (npc_behave)
		{
			if (npc_behave->npc_facing == Behaviour_NPC::NPCFacing::START)
			{
				m_Texture->isRendering = false;
			}
			else
			{
				m_Texture->isRendering = true;
			}

			if (npc_behave->npc_facing == Behaviour_NPC::NPCFacing::RIGHT)
			{
				m_Transform->setRotationAngle(0.f);

				// Update [Cone Sprite Position] -> dependign on where the [NPC] is facing. (RIGHT)
				m_Transform->setTranslate(SEVec2{ npc_translate.x + 100, npc_translate.y });
				// Remember to update the collision box too!
				m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
			}

			if (npc_behave->npc_facing == Behaviour_NPC::NPCFacing::LEFT)
			{
				m_Transform->setRotationAngle(180.0f);

				// Update [Cone Sprite Position] -> dependign on where the [NPC] is facing. (RIGHT)
				m_Transform->setTranslate(SEVec2{ npc_translate.x - 100, npc_translate.y });
				// Remember to update the collision box too!
				m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
			}

			if (npc_behave->npc_facing == Behaviour_NPC::NPCFacing::UP)
			{
				m_Transform->setRotationAngle(90.0f);

				// Update [Cone Sprite Position] -> dependign on where the [NPC] is facing. (UP)
				m_Transform->setTranslate(SEVec2{ npc_translate.x, npc_translate.y + 100 });
				// Remember to update the collision box too!
				m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });

			}

			if (npc_behave->npc_facing == Behaviour_NPC::NPCFacing::DOWN)
			{
				m_Transform->setRotationAngle(270.0f);

				// Update [Cone Sprite Position] -> dependign on where the [NPC] is facing. (UP)
				m_Transform->setTranslate(SEVec2{ npc_translate.x, npc_translate.y - 100 });
				// Remember to update the collision box too!
				m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x, m_Collision->boundaries.scale.y });
			}
		}
	}


}

void Behaviour_Proximity::StopChasing()
{
	chasing = false;
}

void Behaviour_Esclamation::Init()
{
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_currentAction = ACTION::ACTION_EXCLAMATION;
	// npc_name initialized in serialization.
}

void Behaviour_Esclamation::Update()
{
}

void Behaviour_GameplayUI::Init()
{
	GameObject* GoWorld = SEFindObjectByName("Background0");
	Collision* WorldColl = nullptr;
	if (GoWorld)
		WorldColl = (Collision*)GoWorld->getComponent(CT_COLLISION);

	if (WorldColl)
		WorldBB = { WorldColl->boundaries.min.x, WorldColl->boundaries.min.y, WorldColl->boundaries.max.x, WorldColl->boundaries.max.y };

	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = nullptr;
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	WinVec = SEVec2(SEGetWindowWidth() / 2.f, SEGetWindowHeight() / 2.f);
	if (go->getName() == "obj0")
	{
		m_currenAction = ACTION::ACTION_OBJECTIVE_CLOSE;
	}

	if (go->getName() == "Sus0")
	{
		m_currenAction = ACTION::ACTION_NULL;
	}

	if (go->getName() == "TASKBAR_BG0")
	{
		m_currenAction = ACTION::ACTION_ACTIVE;
	}

	if (go->getName() == "TASKBAR_Fill1")
	{
		m_currenAction = ACTION::ACTION_ACTIVE;
	}
	if (go->getName() == "Exit Btn Default39")
	{
		m_currenAction = ACTION::ACTION_OPEN_HOW_TO_PLAY;
	}

	if (go->getName() == "Player_VFX") {
		m_currenAction = ACTION::ACTION_INACTIVE;
		VFXDefScale = m_Transform->getScale();
	}

	if (go->getName() == "Win Screen")
	{
		m_Transform->setScale(SEVec2(450, 250));
	}

	if (go->getName() == "Game Over Screen0")
	{
		m_Transform->setScale(SEVec2(450, 250));
	}


	VFX_PLeft = VFXDefScale.x;
	VFX_PRight = -VFX_PLeft;
}

static bool reset_timer_once = false;

void Behaviour_GameplayUI::Update()
{
	if (go->getName() == "Player_VFX") {
		GameObject* PlayerObj = SEFindObjectByName("Player9");
		Transform* PlayerTrans = (Transform*)PlayerObj->getComponent(CT_TRANSFORM);
		Behaviour_Player* PlayerBehave = (Behaviour_Player*)PlayerObj->m_behaviour.get();

		switch (PlayerBehave->m_CurrentAction)
		{
		case ACTION::ACTION_WALKING_PHOTO:
		case ACTION::ACTION_WALKING_TRASHBAG:
		case ACTION::ACTION_WALKING_PAPER:
		case ACTION::ACTION_WALKING_BROOM:
		case ACTION::ACTION_WALKING:
			m_currenAction = ACTION::ACTION_ACTIVE;
			break;
		case ACTION::ACTION_SEARCHING:
			m_currenAction = ACTION::ACTION_SEARCHING;
			break;
		case ACTION::ACTION_SEARCH_FAKE_HORN:
			m_currenAction = ACTION::ACTION_SEARCH_FAKE_HORN;
			break;
		case ACTION::ACTION_IDLE:
		default:
			m_currenAction = ACTION::ACTION_INACTIVE;
			break;
		}

		SEVec2 tPTRans = PlayerTrans->getTranslate();
		SEVec2 tPScale = PlayerTrans->getScale();

		if (m_currenAction == ACTION::ACTION_ACTIVE) {
			m_Transform->setTranslate({ tPTRans.x - (tPScale.x * 0.7f),tPTRans.y - (tPScale.y * 0.7f) });

			if (m_Transform->getScale().x < tPScale.x)
				m_Transform->setScale({ VFX_PLeft, m_Transform->getScale().y });

			else
				m_Transform->setScale({ VFX_PRight, m_Transform->getScale().y });
		}

		else if (m_currenAction == ACTION::ACTION_SEARCHING) {
			m_Transform->setTranslate({ tPTRans.x,tPTRans.y });
			m_Transform->setScale(SEARCH_VFX_SCALE);

		}
		else if (m_currenAction == ACTION::ACTION_SEARCH_FAKE_HORN) {
			m_Transform->setTranslate({ tPTRans.x,tPTRans.y });
			m_Transform->setScale(SEARCH_VFX_SCALE);

		}
		else {
			m_Transform->setScale(VFXDefScale);
		}
	}

	if (SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_NOTE) && SEGetGameProgress(ProgressFlags_LV1::KEY_DISPOSE_GLASS) && SEGetGameProgress(ProgressFlags_LV1::KEY_REPLACE_PHOTO))
	{
		//TriggerWinScreen();
		SET_LEVEL(LEVEL::LEVEL_2);
	}

	if (SEGetGameProgress(ProgressFlags_LV2::REPLACE_BLOOD_WITH_PAINT) && SEGetGameProgress(ProgressFlags_LV2::CLEAN_BLOOD_ON_KNIFE) && SEGetGameProgress(ProgressFlags_LV2::REPLACE_HORN_WITH_CANDY))
	{
		TriggerWinScreen();
	}

	// Level 2 Checks 
	// [Objective #1 - Replace Blood w Paint] 
	if (SEGetGameProgress(ProgressFlags_LV2::MOP_BLOOD_STAIN) && SEGetGameProgress(ProgressFlags_LV2::WASH_BLOOD_STAIN) && SEGetGameProgress(ProgressFlags_LV2::REPLACE_MOP) &&
		SEGetGameProgress(ProgressFlags_LV2::POUR_FAKE_BLOOD) && SEGetGameProgress(ProgressFlags_LV2::PUT_BACK_EMPTY_BUCKET))
	{
		if (!SEGetGameProgress(ProgressFlags_LV2::REPLACE_BLOOD_WITH_PAINT)) // if false -> toggle to true. 
			ToggleGameProgressLv2(ProgressFlags_LV2::REPLACE_BLOOD_WITH_PAINT); // this will trigger the strikeout in [SELayerStack::SELayerStackDraw()]
	}

	if (currSusMLevel == 5)
	{
		/*	if (trigger_lose == false)
			{*/
		TriggerLoseScreen();
		/*	trigger_lose = true;
		}*/

		if (!reset_timer_once)
		{
			ScriptTimer.Reset();
			reset_timer_once = true;
		}

		if (go->getName() == "Game Over Screen0" && ScriptTimer.Countup(SEGetDeltaTime() * 40))
		{
			m_currenAction = ACTION::ACTION_NULL;
		}

		else if (go->getName() == "Game Over Screen0")
		{
			m_currenAction = ACTION::ACTION_LOSE_SCREEN;
		}
	}

	GameObject* p9obj = SEFindObjectByName("Player9");
	Transform* p9Trans = (Transform*)p9obj->getComponent(CT_TRANSFORM); (void)p9Trans;


	//current_CameraPos = SEVec2(p9Trans->getTranslate().x, p9Trans->getTranslate().y);		// -> this line causes the jittery effect 
	current_CameraPos = { GetCameraPositionX(), GetCameraPositionY() };
	const float& width = (SEFloat)SEGetCameraWidth() / 2.f;
	const float& height = (SEFloat)SEGetCameraHeight() / 2.f;
	const float offset = 150.0f;

	if (go->getName() == "obj0")
	{
		if (SEIsKeyTriggered(SE_KEY_O))
		{
			objective_open = !objective_open;
		}
		m_UIstatus = objective_open;
		//m_Transform->setTranslate(SEVec2(current_CameraPos.x + 630, current_CameraPos.y + 320));
		////m_Transform->setTranslate(SEVec2(current_CameraPos.x + (width / 2.0f - offset), current_CameraPos.y + (height / 2.0f - offset)));
		//m_Transform->setScale(SEVec2(200, 200));
		//Parent_Pos = { m_Transform->getTranslate().x - 140, m_Transform->getTranslate().y + 8 };


		m_Transform->setTranslate(SEVec2(current_CameraPos.x + width - 108, current_CameraPos.y + height - 80));
		// m_Transform->setTranslate(SEVec2(current_CameraPos.x + (width / 2.0f - offset), current_CameraPos.y + (height / 2.0f - offset)));
		Parent_Pos = { m_Transform->getTranslate().x - 80, m_Transform->getTranslate().y + 4 };

		if (!objective_open)
		{
			m_currenAction = ACTION::ACTION_OBJECTIVE_CLOSE;
		}

		else
		{
			m_currenAction = ACTION::ACTION_OBJECTIVE_OPEN;
		}
	}

	// Sus Meter 
	if (Sus_Increase)
	{
		if (go->getName() == "Sus0")
		{
			m_Transform->setTranslate(SEVec2(current_CameraPos.x - width + 58, current_CameraPos.y + height - 44));

			if (ScriptTimer.Countup(0.5))
			{
				m_currenAction = ACTION::ACTION_NULL;
				Sus_Increase = false;
			}

			else
			{
				m_currenAction = ACTION::ACTION_ACTIVE;
			}
		}

	}

	Behaviour_Player* pBehave = (Behaviour_Player*)p9obj->m_behaviour.get();
	GameObject* tObj = nullptr;
	Transform* tTrans = nullptr;
	if (pBehave)
	{
		if (pBehave->m_CurrentAction == ACTION::ACTION_CLEANING_BROOM)
		{
			tObj = SEFindObjectByName("Glass Pieces26");
			tTrans = (Transform*)tObj->getComponent(CT_TRANSFORM);
		}

		if (pBehave->m_CurrentAction == ACTION::ACTION_SEARCHING)
		{
			tObj = SEFindObjectByName("PP Pile51");
			tTrans = (Transform*)tObj->getComponent(CT_TRANSFORM);
		}
		if (pBehave->m_CurrentAction == ACTION::ACTION_SEARCH_FAKE_HORN)
		{
			tObj = SEFindObjectByName("candyPile2");
			tTrans = (Transform*)tObj->getComponent(CT_TRANSFORM);
		}

		if (pBehave->m_CurrentAction == ACTION::ACTION_CLEANING_MOP || pBehave->m_CurrentAction == ACTION::ACTION_POURING_PAINT)
		{
			tObj = SEFindObjectByName("pinkblood3");
			tTrans = (Transform*)tObj->getComponent(CT_TRANSFORM);
		}

		if (pBehave->m_CurrentAction == ACTION::ACTION_WASHING)
		{
			tObj = SEFindObjectByName("Sink21");
			tTrans = (Transform*)tObj->getComponent(CT_TRANSFORM);
		}

		//if (pBehave->m_CurrentAction == ACTION::ACTION_WASHING) {
		//	tObj = SEFindObjectByName("Sink21");
		//	tTrans = (Transform*)tObj->getComponent(CT_TRANSFORM);
		//}
	}

	if (go->getName() == "TASKBAR_Fill1")
	{
		if (pBehave->m_CurrentAction == ACTION::ACTION_SEARCHING || pBehave->m_CurrentAction == ACTION::ACTION_CLEANING_BROOM || pBehave->m_CurrentAction == ACTION::ACTION_WASHING ||
			pBehave->m_CurrentAction == ACTION::ACTION_CLEANING_MOP || pBehave->m_CurrentAction == ACTION::ACTION_POURING_PAINT || pBehave->m_CurrentAction == ACTION::ACTION_SEARCH_FAKE_HORN)
		{
			m_currenAction = ACTION::ACTION_ACTIVE;
			SEVec2 tempsc = m_Transform->getScale();
			double TBshift = tTrans->getTranslate().x - (MAX_BAR_SCALE / 2.0);
			double TBnewscale = ScriptTimer.getCurrentPctg() * MAX_BAR_SCALE;
			double TBoffset = TBshift + TBnewscale;
			m_Transform->setScale((float)TBnewscale, tempsc.y);
			m_Transform->setTranslate({ TBoffset - (MAX_BAR_SCALE / 2.0), tTrans->getTranslate().y + 50 });
		}

		else
		{
			m_currenAction = ACTION::ACTION_INACTIVE;
		}
	}

	if (go->getName() == "TASKBAR_BG0")
	{
		if (pBehave->m_CurrentAction == ACTION::ACTION_SEARCHING || pBehave->m_CurrentAction == ACTION::ACTION_CLEANING_BROOM || pBehave->m_CurrentAction == ACTION::ACTION_CLEANING_MOP || pBehave->m_CurrentAction == ACTION::ACTION_WASHING
			|| pBehave->m_CurrentAction == ACTION::ACTION_POURING_PAINT || pBehave->m_CurrentAction == ACTION::ACTION_SEARCH_FAKE_HORN)
		{
			m_currenAction = ACTION::ACTION_ACTIVE;
			m_Transform->setTranslate({ tTrans->getTranslate().x, tTrans->getTranslate().y + 50 });
		}

		else
		{
			m_currenAction = ACTION::ACTION_INACTIVE;
		}
	}

	if (go->getName() == "Sus0")
	{
		m_Transform->setTranslate(SEVec2(current_CameraPos.x - width + 58, current_CameraPos.y + height - 44));
		if (Sus_Increase) {
			m_currenAction = ACTION::ACTION_GLOW;
		}
	}

	if (go->getName() == "HTP Bg38_Level_1")
	{
		m_Transform->setTranslate(SEVec2(current_CameraPos.x, current_CameraPos.y));
	}

	// if (go->getName() == "HTP Bg38")
	//{
	//	// Access Player's holding go find his behaviour script
	//	GameObject* player = SEFindObjectByName("Player9");

	//	if (player)
	//	{
	//		Behaviour_Player* player_behave = dynamic_cast<Behaviour_Player*>(player->m_behaviour.get());

	//	}
	//
	//}

	SusArrowRotate();

	if (go->getName() == "Game Over Screen0")
	{
		if (m_Transform)
		{
			m_Transform->setTranslate(SEVec2(current_CameraPos.x, current_CameraPos.y));
		}
	}

	if (go->getName() == "Win Screen")
	{
		if (m_Transform)
		{
			m_Transform->setTranslate(SEVec2(current_CameraPos.x, current_CameraPos.y));
		}
	}

}

void Behaviour_GameplayUI::SusGlow()
{
	if (go->getName() == "Sus0")
	{
		m_currenAction = ACTION::ACTION_GLOW;
	}
}

void Behaviour_GameplayUI::SusStopGlow()
{
	if (go->getName() == "Sus0")
	{
		m_currenAction = ACTION::ACTION_NULL;
	}
}

void Behaviour_Arrow::Init()
{
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	m_currentAction = ACTION::ACTION_INACTIVE;

}

void Behaviour_Arrow::Update()
{

}

void Behaviour_GameplayUI::SusArrowRotate()
{
	GameObject* sus_arrow_go = SEFindObjectByName("Sus Arrow");
	GameObject* sus_metre_go = SEFindObjectByName("Sus0");
	Transform* trans_arrow = (Transform*)sus_arrow_go->getComponent(CT_TRANSFORM);
	Transform* trans_metre = (Transform*)sus_metre_go->getComponent(CT_TRANSFORM);
	SEVec2 Meter_Pos{};

	if (trans_metre) {
		Meter_Pos = trans_metre->getTranslate();
	}


	if (sus_arrow_go)
	{
		if (trans_arrow)
		{
			switch ((int)currSusMLevel)
			{
			case 0:															 // default
				trans_arrow->setTranslate({ Meter_Pos.x + (-10), Meter_Pos.y + (-30) }); // OK
				trans_arrow->setRotationAngle(0.f);
				break;
			case 1:
				trans_arrow->setTranslate({ Meter_Pos.x + (7.5), Meter_Pos.y + (-21) });
				trans_arrow->setRotationAngle(45.f);
				break;
			case 2:
				trans_arrow->setTranslate({ Meter_Pos.x + (16), Meter_Pos.y + (-9) });
				trans_arrow->setRotationAngle(70.f);
				break;
			case 3:
				trans_arrow->setTranslate({ Meter_Pos.x + (17.5), Meter_Pos.y + (10) });
				trans_arrow->setRotationAngle(105.f);
				break;
			case 4:
				trans_arrow->setTranslate({ Meter_Pos.x + (9), Meter_Pos.y + (24) });
				trans_arrow->setRotationAngle(135.f);
				break;
			case 5:
				trans_arrow->setTranslate({ Meter_Pos.x + (-10), Meter_Pos.y + (32) });
				trans_arrow->setRotationAngle(180.0f);
				break;
			}

			if ((SEIsKeyTriggered(SE_KEY_X)))
			{
				currSusMLevel = 0;
			}
			if ((SEIsKeyTriggered(SE_KEY_C)))
			{
				currSusMLevel = 1;
			}

			if (SEIsKeyTriggered(SE_KEY_V))
			{
				currSusMLevel = 2;
			}

			if (SEIsKeyTriggered(SE_KEY_B))
			{
				currSusMLevel = 3;
			}

			if (SEIsKeyTriggered(SE_KEY_N))
			{
				currSusMLevel = 4;
			}
#if 1
			if (SEIsKeyTriggered(SE_KEY_M))
			{
				currSusMLevel = 5;
				GameObject* go_retry = SEFindObjectByName("GO Retry1");
				GameObject* go_exit = SEFindObjectByName("GO Exit2");

				if (go_retry)
				{
					Collision* col_retry = (Collision*)go_retry->getComponent(CT_COLLISION);
					if (col_retry)
					{
						col_retry->isActive = true;
					}

					Collision* col_exit = (Collision*)go_exit->getComponent(CT_COLLISION);
					if (col_exit)
					{
						col_exit->isActive = true;
					}
				}
			}
#endif
		}
	}
}

void Behaviour_GameplayUI::TriggerLoseScreen()
{
	GameObject* gameover_go = SEFindObjectByName("Game Over Screen0");
	GameObject* retrybtn_go = SEFindObjectByName("GO Retry1");
	GameObject* exitbtn_go = SEFindObjectByName("GO Exit2");


	if (gameover_go)
	{
		Texture* gameover_text = (Texture*)gameover_go->getComponent(CT_TEXTURE);
		if (gameover_text)
		{
			gameover_text->isRendering = true;
		}
	}

	if (retrybtn_go)
	{
		Texture* retrybtn_text = (Texture*)retrybtn_go->getComponent(CT_TEXTURE);
		Collision* collision_retry = (Collision*)retrybtn_go->getComponent(CT_COLLISION);

		if (retrybtn_text)
			retrybtn_text->isRendering = true;
		if (collision_retry)
			collision_retry->isActive = true;
	}

	if (exitbtn_go)
	{
		Texture* exitbtn_text = (Texture*)exitbtn_go->getComponent(CT_TEXTURE);
		Collision* collision_exit = (Collision*)exitbtn_go->getComponent(CT_COLLISION);

		if (exitbtn_text)
			exitbtn_text->isRendering = true;

		if (collision_exit)
			collision_exit->isActive = true;
	}

}


void Behaviour_GameplayUI::TriggerWinScreen()
{
	GameObject* winscreen_go = SEFindObjectByName("Win Screen");
	GameObject* win_button_go = SEFindObjectByName("WIN MM BTN2");

	if (winscreen_go)
	{
		Texture* gameover_text = (Texture*)winscreen_go->getComponent(CT_TEXTURE);
		if (gameover_text)
		{
			gameover_text->isRendering = true;
		}
	}

	if (win_button_go)
	{
		Texture* retrybtn_text = (Texture*)win_button_go->getComponent(CT_TEXTURE);
		Collision* collision_retry = (Collision*)win_button_go->getComponent(CT_COLLISION);

		if (retrybtn_text)
			retrybtn_text->isRendering = true;
		if (collision_retry)
			collision_retry->isActive = true;
	}

}

SEVec2 Bounded_Cam(SEVec2 _translate, SEVec2 _Cam_dimensions, SEVec4 _WorldBound) {
	SEVec2
		topleft = { _translate.x - _Cam_dimensions.x , _translate.y + _Cam_dimensions.y },
		bottomleft = { _translate.x - _Cam_dimensions.x, _translate.y - _Cam_dimensions.y },
		bottomright = { _translate.x + _Cam_dimensions.x, _translate.y - _Cam_dimensions.y },
		topright = { _translate.x + _Cam_dimensions.x, _translate.y + _Cam_dimensions.y },
		final_translate = _translate;

	//x collisions(-ve x)
	if (topleft.x < _WorldBound.x || bottomleft.x < _WorldBound.x) {
		final_translate.x = _WorldBound.x + _Cam_dimensions.x;
	}

	//y collisions(-ve y)
	if (bottomleft.y < _WorldBound.y || bottomright.y < _WorldBound.y) {
		final_translate.y = _WorldBound.y + _Cam_dimensions.y;
	}

	//z collisions (+ve x)
	if (topright.x > _WorldBound.z || bottomright.x > _WorldBound.z) {
		final_translate.x = _WorldBound.z - _Cam_dimensions.x;
	}

	//w collisions (+ve y)
	if (topright.y > _WorldBound.w || topleft.y > _WorldBound.w) {
		final_translate.y = _WorldBound.w - _Cam_dimensions.y;
	}


	return final_translate;
}


void Behaviour_Splash::Init() {
	if (go->getName() == "DigipenLogo") {
		m_currentAction = ACTION::ACTION_ACTIVE;
		m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	}

	if (go->getName() == "FmodLogo") {
		m_currentAction = ACTION::ACTION_INACTIVE;
		m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	}

	if (go->getName() == "IPSLogo") {
		m_currentAction = ACTION::ACTION_INACTIVE;
		m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	}

}


void Behaviour_Splash::Update()
{
	if (go->getName() == "DigipenLogo" && splashqueue == 0) {


		if (m_DigiTimer >= 4.5f) {
			m_currentAction = ACTION::ACTION_INACTIVE;
			splashqueue++;

		}

		else {
			m_DigiTimer += 0.25f * (float)SEGetDeltaTime();
			if (m_Transform)
				m_Transform->setScale(m_Transform->getScale().x * (1.f + (1.f / 1000.f)), m_Transform->getScale().y * (1.f + (1.f / 1000.f)));
		}


	}
	//std::cout << "DigiTimer:" << m_DigiTimer << "| " << (int)m_currentAction << std::endl;
	if (go->getName() == "FmodLogo" && splashqueue == 1) {
		if (m_FMODTimer >= 3.f) {
			m_currentAction = ACTION::ACTION_INACTIVE;
			splashqueue++;
		}

		else {
			m_currentAction = ACTION::ACTION_ACTIVE;
			m_FMODTimer += 1.f * (float)SEGetDeltaTime();
			if (m_Transform)
				m_Transform->setScale(m_Transform->getScale().x * (1.f + (1.f / 1000.f)), m_Transform->getScale().y * (1.f + (1.f / 1000.f)));
		}


	}
	//std::cout << "FMODTimer:" << m_FMODTimer << " | " << (int)m_currentAction << std::endl;
	if (go->getName() == "IPSLogo" && splashqueue == 2) {
		if (m_IPSTimer >= 3.f) {
			m_currentAction = ACTION::ACTION_INACTIVE;
			p_Scene->SetCurrentScene(LEVEL::MAIN_MENU);
		}

		else {
			m_currentAction = ACTION::ACTION_ACTIVE;
			m_IPSTimer += 1.f * (float)SEGetDeltaTime();
			if (m_Transform)
				m_Transform->setScale(m_Transform->getScale().x * (1.f + (1.f / 1000.f)), m_Transform->getScale().y * (1.f + (1.f / 1000.f)));
		}


	}
	//std::cout << "FMODTimer:" << m_FMODTimer << " | " << (int)m_currentAction << std::endl;


//if (queue == 1) {
//	if (FmodTimer.Countup(5.0)) {
//		p_Scene->SetCurrentScene(LEVEL::MAIN_MENU);
//	}

//	else {
//		if (go->getName() == "FmodLogo") {
//			if (m_Transform)
//				m_Transform->setScale(m_Transform->getScale().x * 1.1, m_Transform->getScale().y * 1.1);
//		}
//		std::cout << "Fmod: " << FmodTimer.getCurrentTime() << std::endl;
//	}
//}
}

void Behaviour_Credits::Init() {
	creditsqueue = 1;
	m_Transform = (Transform*)go->getComponent(CT_TRANSFORM);
	m_Collision = (Collision*)go->getComponent(CT_COLLISION);
	m_Texture = (Texture*)go->getComponent(CT_TEXTURE);
	if (go->getName() == "Credits Left Arrow") {
		m_currentAction = ACTION::ACTION_ACTIVE;
	}
	if (go->getName() == "Credits Right Arrow") {
		m_currentAction = ACTION::ACTION_ACTIVE;
	}
	if (go->getName() == "Credits Exit Button") {
		m_currentAction = ACTION::ACTION_BUTTON_IDLE;
	}
}

void Behaviour_Credits::Update() {

	auto MousePosition = SEGetCursorInWorld();
	glm::vec2 Mouse = { MousePosition.x, MousePosition.y };
	//if (p_Scene->GetCurrentScene() == LEVEL::CREDITS) {

	if (m_Collision) {
		m_Collision->SetCollision(m_Transform->getTranslate(), { m_Collision->boundaries.scale.x,m_Collision->boundaries.scale.y });
		if (SECheckCollision(m_Collision, Mouse)) {
			if (go->getName() == "Credits Exit Button")
			{
				m_currentAction = ACTION::ACTION_BUTTON_ISHOVERED;
				if (m_Texture->AnimationHandler.i > m_Texture->AnimationFrames[m_currentAction] - 3)
					m_Texture->AnimationHandler.i = m_Texture->AnimationFrames[m_currentAction] - 2;
				if (m_Texture->AnimationHandler.i < 0)
					m_Texture->AnimationHandler.i = 0;
			}

			if (SEIsMouseLeftPressed()) {
				SEPlaySound(AUDIOTYPE::SFX, "bubblepop.mp3");

				if (go->getName() == "Credits Left Arrow") {
					if (m_currentAction == ACTION::ACTION_ACTIVE) {
						creditsqueue--;
						//valuechange = true;
					}
				}

				if (go->getName() == "Credits Right Arrow") {
					if (m_currentAction == ACTION::ACTION_ACTIVE) {
						creditsqueue++;
						//valuechange = true;
					}
				}

				if (go->getName() == "Credits Exit Button") {
					p_Scene->SetCurrentScene(LEVEL::MAIN_MENU);
				}
			}
		}

		else {
			if (go->getName() == "Credits Exit Button") m_currentAction = ACTION::ACTION_BUTTON_IDLE;
		}
	}


	if (go->getName() == "Credits Left Arrow") {
		if (creditsqueue == 1) {
			m_currentAction = ACTION::ACTION_INACTIVE;
		}

		else {
			m_currentAction = ACTION::ACTION_ACTIVE;
		}
	}

	if (go->getName() == "Credits Right Arrow") {
		if (creditsqueue == 8) {
			m_currentAction = ACTION::ACTION_INACTIVE;
		}

		else {
			m_currentAction = ACTION::ACTION_ACTIVE;
		}
	}

	if (go->getName() == "Credits 1") {
		if (creditsqueue == 1) m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}
	if (go->getName() == "Credits 2") {
		if (creditsqueue == 2) m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}
	if (go->getName() == "Credits 3") {
		if (creditsqueue == 3)m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}
	if (go->getName() == "Credits 4") {
		if (creditsqueue == 4) m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}
	if (go->getName() == "Credits 5") {
		if (creditsqueue == 5) m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}
	if (go->getName() == "Credits 6") {
		if (creditsqueue == 6)m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}
	if (go->getName() == "Credits 7") {
		if (creditsqueue == 7) m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}
	if (go->getName() == "Credits 8") {
		if (creditsqueue == 8) m_currentAction = ACTION::ACTION_ACTIVE;
		else m_currentAction = ACTION::ACTION_INACTIVE;
	}

	//if (valuechange) {
	//	std::cout << "Credits Queue: " << creditsqueue << std::endl;
	//	valuechange = false;
	//}
	//}
}


