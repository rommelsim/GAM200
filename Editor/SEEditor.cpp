/******************************************************************************//**
 * \file			SEEditor.cpp
 * \author 			Loke Kheng Ray
 * \co-author		Chua Kai Quan
 * \contributions	Loke Kheng Ray	    (85%)
 *					Kai Quan			(15%)
 * \par    	l.khengray@digipen.edu
 * \par     zhenpengrommel.sim@digipen.edu (100%)
 * \brief	This file contains the declaration for editor.
 * \date   	January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#include "pch.h"
#include "SEEditor.h"
#include <SELayerStack.h>

// Windows own window handler
#include <commdlg.h>
#include <glfw3native.h>

enum class LEVEL;

// Window variables
GLFWwindow* pWindow;
ImVec2 viewportPanelSize;
ImVec2 viewportOffset;
ImVec2 viewportBounds[2];
static float offsetX = 0.0f, offsetY = 0.0f;
static float new_VPWidth, new_VPHeight;
#pragma region IMGUI WINDOWS VARIABLES

// bool for individual imgui windows
bool fileWind_open = false;
bool settings_open = false;
bool inspector_open = false;
bool createPrefab_open = false;
bool addCompoWind_open = false;
bool notifWindow_open = false;
bool setTextureWindow_open = false;
bool dialogueManager_open = false;
bool renameObject_open = false;

// Notif Window settings
ImVec2 NotifWindowSize = ImVec2(0, 0);
ImVec2 NotifWindowPos = ImVec2(0, 0);
std::string NotifWindowMessage = "";

#pragma endregion IMGUI WINDOWS VARIABLES


#pragma region IMGUI COMPONENTSECTION VARIABLES

// Drop down display for anything using Component List
static std::string previewComponent = "";

// Component List
static std::vector<const char*> componentList      // TODO : Component currently in Enum, need string to reflect in imgui
{
    "Transform",
    "Collision",
    "Texture",
    "Dialogue",
    "ButtonGUI",
    "Evidence",     // TODO : Create Component Section for inspector
    "Tools"         // TODO : Create Component Section for inspector
};
static bool componentCursor[7];     // To include components that are selected when creating
static bool deleteCompCursor[7];    // To check components to delete

// Cursor position to begin drawing UI of component details
// cursor(_yPos) helps with the padding of the UI on the inspector
float inspector_yPos = 30.f;
// height of each component section displayed in window
const int colorComponent_height = 350;
const int transformComponent_height = 240;
const int collisionComponent_height = 180;
const int textureComponent_height = 200;
const int controllerComponent_height = 240;
const int audioComponent_height = 170;
const int AIComponent_height = 350;
const int buttonGUI_height = 350;
const int dialogue_height = 150;
const int textComp_height = 240;
const int waypoints_height = 120;

// bool for component section 
bool colorSection_active = false;
bool transformSection_active = false;
bool collisionSection_active = false;
bool textureSection_active = false;
bool controllerSection_active = false;
bool audioSection_active = false;
bool AISection_active = false;
bool SpriteSection_active = false;
bool ButtonGUISection_active = false;
bool DialogueSection_active = false;
bool TextCompSection_active = false;
bool WaypointsSection_active = false;
bool LoadingWindow_active = false;

#pragma endregion IMGUI COMPONENTSECTION VARIABLES


#pragma region IMGUI PREFAB CREATION VARIABLES

// Prefab Gameobject Container. Kill when exit.
std::vector<GameObject*> prefab_container_go;
//std::vector<GameObject*> prefab_container_overlay;
//std::vector<GameObject*> prefab_container_uielems;
//std::vector<GameObject*> prefab_container_foregnd;
//std::vector<GameObject*> prefab_container_gameply;
//std::vector<GameObject*> prefab_container_backgnd;

// Prefab Button Size
ImVec2 ButtonSize = ImVec2(80, 80);
// Prefab buttons per Column for the palette window
int buttons_perColumn = 3;

// For creating new prefab
static char newPrefabName[64] = "";
static bool setdeleteComponent = false;     // If selected, enable delete component in CreatePrefab

// Map is not used as our intended outcome is to sort by creation order instead of by name (Map auto sorts by name)
// Also need a container for bool to check which prefab user is creating. (and turn off all others)
std::vector<std::string>		PrefabsContainer{};
std::vector<bool>               PrefabCreateSwitches{};


#pragma endregion IMGUI PREFAB CREATION VARIABLES


#pragma region IMGUI ANIMATIONSECTION VARIABLES

// Get Available Animation states to reflect in the editor
static std::vector<const char*>  AnimationState     // TODO : GET AVAILABLE ANIM LIST
{
    "Idle",
    "Walking",
    "Alert",
    "Cleaning"
};
static bool AnimStateCursor[4];

// Animation variables and data
static bool is_SpriteSheet = false;            // TODO : GET IF CURRENT SELECTED OBJ IS USING SPRITE SHEET
static int currentFrame = 69;           // TODO : GET CURRENT FRAME FROM CURRENT SPRITE SHEET
static int renderPriority = 5;          // TODO : GET RENDER PRIORITY               [Done]
static std::vector<std::string> spriteList{};       // Initialized in SE_UIInit()   [Done]
bool* spriteCursor;
//static bool spriteCursor[5];
//static std::vector<bool> spriteCursor{};

#pragma endregion IMGUI ANIMATIONSECTION VARIABLES


#pragma region IMGUI GUIFUNCTION VARIABLES
// Animation functionality for buttonGUI
bool animHover = false;
bool animClick = false;

static std::vector<const char*>  GUIFuncList     // TODO : GET AVAILABLE FUNCTION LIST
{
    "Change Scene",
    "Toggle UI",
    "Change Dialogue",
    "Add Suspicion",
    "Reduce Suspicion",
    "Pause Game",
    "Quit Game"
};
static bool GUIFuncCursor[7];

static std::vector<const char*>  GUI_UIList     // TODO : GET AVAILABLE UI LIST
{
    "Pause UI",
    "Tutorial UI",
    "Objective UI"
};
static bool GUIUICursor[7];

#pragma endregion IMGUI GUIFUNCTION VARIABLES

#pragma region DIALOGUE WINDOWS DATA

// Unique Label for ImGui (each Imgui needs unique identifier)
/*
*   Each ImGui function utilizes a char array called "label" which
*   acts as an identifier to check between 2 separate ImGui functions.
*   Hence, there can be no 2 exact same ImGui functions with exact
*   same identifier.
*   This issue is rectified by having a standard string along with
*   an ID to append to the string. For each new "Dialogue Section",
*   this ID will be incremented so there will no 2 exact identifiers.
*/
static size_t      DialogueID{};       // Something unique to each dialogue Node (as an identifier)
static int      DialoguePos{};      // Position defines where the UI is being displayed
std::vector<std::string> DiaName{};
std::vector<std::string> DiaContent{};
std::vector<std::string> DiaChoiceA{};
std::vector<std::string> DiaChoiceB{};
std::vector<std::string> DiaJumpA{};
std::vector<std::string> DiaJumpB{};
static std::vector<std::string> DiaPreviewJumpA{};
static std::vector<std::string> DiaPreviewJumpB{};
std::vector<std::string> SusModA{};
std::vector<std::string> SusModB{};
std::vector<std::string> DiaDeleteCursor{};
std::vector<std::string> DiaSaveBtn{};
static bool DeleteDiaFlag = false;

// List of Dialogue Node Names : Acts as an identifier to jump to next dialogue
static std::vector<std::string> diaNodeList{ "None" , "NodeTESTA", "NodeTESTB" , "NodeTESTC" };
bool* diaNodeACursor;
bool* diaNodeBCursor;

// Layout Variables
float choiceBoxSize = 230.f;

#pragma endregion DIALOGUE WINDOWS DATA


// For show/hiding all layer objs of hierachy
bool hier_showall = false;


static std::string previewLevelName = "";       // For Imgui default display            (Possibly Deprecated due to open from file settings)
static std::vector<const char*>  LevelList     // TODO : GET AVAILABLE SPRITE LIST      (Possibly Deprecated due to open from file settings)
{
    "Level 1",
    "Level 2",
    "Level 3",
    "Level 4",
    "Level 5"
};
static bool levelCursor[5];



GameObject* current_GO;
GameObject* current_prefab_GO;
static char currGO_newname[64] = "";

ImGuiContext* wContext = nullptr;
static std::string rendLayerString = "Background";    // Displayed string on Render selection window
static MOUSE_STATE currMouse = MOUSE_STATE::M_IDLE;
static RENDERLAYER_STATE currRender = RENDERLAYER_STATE::RL_BACKGROUND;

LEVEL currentLevelLoaded = GetLevelForJSON();

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void SE_UIInit()
{
    pWindow = SEGetGLWindow();
    const char* glsl_version = "#version 450";
    IMGUI_CHECKVERSION();
    wContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = "./Assets/Config/imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    // Setup Platform/Renderer bindingimgui
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    InitializeAvailablePrefabs();


    // Init all texture sprites loading 
    std::string myPath = "./Assets/Textures";
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(myPath))
    {
         //std::cout << "Loading " << dirEntry.path() << std::endl;
         //SELoadTexture(dirEntry.path());
        spriteList.push_back(dirEntry.path().string());
    }
    spriteCursor = new bool[spriteList.size()];
    diaNodeACursor = new bool[diaNodeList.size()];
    diaNodeBCursor = new bool[diaNodeList.size()];
    //spriteCursor.resize(spriteList.size(), 0);
}

void InitializeAvailablePrefabs()
{
    // TODO: Rommel comment out all of these because preset-prefabs are included in Prefabs.json
    //PrefabsContainer.push_back("Player");
    //PrefabCreateSwitches.push_back(false);
    //
    //PrefabsContainer.push_back("Bunny");
    //PrefabCreateSwitches.push_back(false);
    //
    //PrefabsContainer.push_back("Deer");
    //PrefabCreateSwitches.push_back(false);
    //
    //PrefabsContainer.push_back("Broken\nGlass");
    //PrefabCreateSwitches.push_back(false);
    //
    //PrefabsContainer.push_back("Broom");
    //PrefabCreateSwitches.push_back(false);
    //
    //PrefabsContainer.push_back("Jack\nin a\nBox");
    //PrefabCreateSwitches.push_back(false);
    // TODO : Rommel
    // Read from file and push into container
    /************************************************
    *          INIT PREFABS
    ************************************************/
    //PrefabsContainer.push_back("Player");
    //PrefabCreateSwitches.push_back(false);

    // Temp storage to dump all my prefabs in...

    SEDeserialiseJSON(prefab_container_go, PREFAB_FILE);

    for (const auto& obj : prefab_container_go)
    {
        (void)obj;
        //Texture* t = (Texture*)obj->getComponent(CT_TEXTURE);

        /*if (t->GetCurrentRenderLayer() == "Background")
        {
            prefab_container_backgnd.push_back(obj);
        }
        if (t->GetCurrentRenderLayer() == "Foreground")
        {
            prefab_container_foregnd.push_back(obj);
        }
        if (t->GetCurrentRenderLayer() == "UI Elements")
        {
            prefab_container_uielems.push_back(obj);
        }
        if (t->GetCurrentRenderLayer() == "Gameplay")
        {
            prefab_container_gameply.push_back(obj);
        }
        if (t->GetCurrentRenderLayer() == "Overlay")
        {
            prefab_container_overlay.push_back(obj);
        }*/



        //PrefabsContainer.push_back(obj->m_name); // all prefabs in the engine will be loaded into here!
        PrefabCreateSwitches.push_back(false);
    }


    // TODO: Add new created prefabs into prefab_container_go
    // Must include all components of the gameobject into prefab list
    // Serialise upon imgui exit.

    // Read all sprites available in ./Assets/Textures/
    //auto tmp = SEGetAllTexturesFrom("./Assets/Textures/");
}

void HandleMouseState()
{
    if (!(CheckHoveredViewport()))
        return;

    if (SEIsMouseLeftPressed())
    {
        glm::vec3 MouseIMgui = SEGetImguiMouse();

        //if (currMouse == MOUSE_STATE::M_SELECT)
        //{

        //    if (current_GO)
        //    {
        //        Transform* tempTrans = (Transform*)current_GO->getComponent(CT_TRANSFORM);
        //        Collision* tempColl = (Collision*)current_GO->getComponent(CT_COLLISION);

        //        if (tempColl == nullptr)
        //            return;

        //        if (SECheckCollision(tempColl, MouseIMgui))
        //        {
        //            tempTrans->setTranslate(SEVec2(MouseIMgui.x, MouseIMgui.y));
        //        }
        //        else
        //        {
        //            //inspector_open = false;
        //            //current_GO = nullptr;
        //            //currMouse = MOUSE_STATE::M_IDLE;
        //        }
        //    }


        //}
        if (currMouse == MOUSE_STATE::M_CREATING)
        {
            // Mouse is Creating Prefabs! 
            SpawnPrefab();
            inspector_open = false;
            current_GO = nullptr;

            std::cout << "CREATING STATE \n";

        }
        else
        {

            // check mouse position with all objects
            for (auto layer_go : SELayerStackManager->m_layer_stack)
            {
                for (auto go : layer_go->GetLayerObjects())
                {
                    if (go)
                    {
                        Collision* tempColl = (Collision*)go->getComponent(CT_COLLISION);

                        if (tempColl)
                        {
                            if (SECheckCollision(tempColl, MouseIMgui))
                            {
                                //std::cout << "collision found! \n";
                                //std::cout << "Obj : " << go->getName() << std::endl;
                                //std::cout << "Mouse : " << MouseIMgui.x << " , " << MouseIMgui.y << std::endl;

                                current_GO = go;
                                current_prefab_GO = nullptr;
                                inspector_open = true;
                                currMouse = MOUSE_STATE::M_SELECT;
                            }
                        }
                    }
                }
            }


            std::cout << "IDLE STATE \n";
            return;
        }
    }


}

static std::string previewCreateNewPrefabSprite = "";
void SpawnPrefab()
{
    // Selected prefab index
    //int selPref = -1;
    if (current_prefab_GO)
    {
        /*std::string name = current_prefab_GO->m_name;
        glm::vec3 MouseIMgui = SEGetImguiMouse();
        SECreateObject<LEVEL::LEVEL_1>(name, "./Assets/Textures/Character_NPC_Victim_Sprite.png", 0, MouseIMgui.x, MouseIMgui.y);
        std::cout << name << std::endl;*/


        glm::vec3 MouseIMgui = SEGetImguiMouse();
        SEPlaySound(AUDIOTYPE::SFX, std::string("bubblepop.mp3"));

        // id is for multiple instances of same objs (2 objs cant share same name)
        static int id = 0;
        std::string name = current_prefab_GO->m_name + std::to_string(id++);
        std::cout << name << std::endl;

        Texture* textureComponent = (Texture*)current_prefab_GO->getComponent(CT_TEXTURE);

        if (textureComponent)
        {
            std::string Rendname = textureComponent->GetCurrentRenderLayer();
            std::cout << Rendname << std::endl;

            //textureComponent->AnimationTextures.string() = previewCreateNewPrefabSprite;
            SERenderLayer spawnLayer = SERenderLayer::OTHERS;
            switch (currRender)
            {
            case RENDERLAYER_STATE::RL_OVERLAY:
                spawnLayer = SERenderLayer::OVERLAY;
                break;
            case RENDERLAYER_STATE::RL_UIELEMENTS:
                spawnLayer = SERenderLayer::UI_ELEMENTS;
                break;
            case RENDERLAYER_STATE::RL_FOREGROUND:
                spawnLayer = SERenderLayer::FOREGROUND;
                break;
            case RENDERLAYER_STATE::RL_GAMEPLAY:
                spawnLayer = SERenderLayer::GAMEPLAY;
                break;
            case RENDERLAYER_STATE::RL_BACKGROUND_OBJ:
                spawnLayer = SERenderLayer::BACKGROUND;
                break;
            case RENDERLAYER_STATE::RL_BACKGROUND:
                spawnLayer = SERenderLayer::BACKGROUND;
                break;
            case RENDERLAYER_STATE::RL_OTHERS:
                spawnLayer = SERenderLayer::OTHERS;
                break;
            default:
                spawnLayer = SERenderLayer::GAMEPLAY;
                break;
            }

            //if (textureComponent->IsAnimating)
            //{
            //    SECreateObject<LEVEL::LEVEL_1>(name, spawnLayer, textureComponent->AnimationTextures, true, MouseIMgui.x, MouseIMgui.y);

            //    SELayerStackManager->SEFindLayer(rendLayerString)->AddLayerObject(SEFindObjectByName(name));
            //}
            //else
            //{
            //    SECreateObject<LEVEL::LEVEL_1>(name, spawnLayer, textureComponent->AnimationTextures, false, MouseIMgui.x, MouseIMgui.y);
            //    SELayerStackManager->SEFindLayer(rendLayerString)->AddLayerObject(SEFindObjectByName(name));
            //}
            std::filesystem::path tmp;
            std::unique_ptr<Texture> tmptex = std::make_unique<Texture>();

            tmptex->AnimationFrames[ACTION::ACTION_NULL] = 1;
            tmptex->AnimationSpeed[ACTION::ACTION_NULL] = 1.0f;
            tmptex->Rows[ACTION::ACTION_NULL] = 1;
            tmptex->Columns[ACTION::ACTION_NULL] = 1;

            // todo: currentgo->texture and newPref->texture not the same!
            std::filesystem::path path = textureComponent->AnimationTextures[ACTION::ACTION_NULL].begin()->second;
            //if (path.empty())
            //    path = textureComponent->AnimationTextures[ACTION::ACTION_NULL].begin()->second;       // naive fix for now. by rommel 22/2/23  

           /* std::unordered_map<std::string, std::filesystem::path> map{};
            map.emplace(path.filename(), path);*/

            tmptex->AnimationTextures[ACTION::ACTION_NULL].emplace(std::make_pair(path.filename().string(), path));
            tmptex->AnimationUV[ACTION::ACTION_NULL] = tmptex->SEComputeUV(tmptex->Rows[ACTION::ACTION_NULL], tmptex->Columns[ACTION::ACTION_NULL], tmptex->AnimationFrames[ACTION::ACTION_NULL]);
            std::unique_ptr<Texture> pTexture = std::make_unique<Texture>();
            pTexture->m_render_layer = spawnLayer;
            SELoadTexture(tmptex->AnimationTextures[ACTION::ACTION_NULL].begin()->second);
            for (const auto& path2 : tmptex->AnimationTextures)
            {
                ACTION action = path2.first;
                std::unordered_map<std::string, std::filesystem::path> map = path2.second;

                if (action == ACTION::ACTION_NULL)
                {
                    auto it = map.begin();

                    // Search for duplicate names and rename them with "_Numbr"
                    std::unordered_multimap<std::string, GameObject*> FullContainer = SEFindObjectsByLevel(LEVEL::LEVEL_1);
                    for (auto obj : FullContainer)
                    {
                        if (name == obj.first)
                        {
                            name = name + "_" + std::to_string(id++);
                        }
                    }

                    //SECreateObject<LEVEL::LEVEL_1>(name, spawnLayer, it->second, MouseIMgui.x, MouseIMgui.y);
                    //SECreateObject<LEVEL::LEVEL_1>(name, spawnLayer, tmptex, MouseIMgui.x, MouseIMgui.y);
                    currentLevelLoaded = GetLevelForJSON();
                    SECreateObject(currentLevelLoaded, name, spawnLayer, tmptex, MouseIMgui.x, MouseIMgui.y);
                    SELayerStackManager->SEFindLayer(rendLayerString)->AddLayerObject(SEFindObjectByName(name));
                }
            }
            /*   SECreateObject<LEVEL::LEVEL_1>(name, spawnLayer, textureComponent->AnimationTextures.at(ACTION::ACTION_NULL)., true, MouseIMgui.x, MouseIMgui.y);
               SELayerStackManager->SEFindLayer(rendLayerString)->AddLayerObject(SEFindObjectByName(name));*/

        }

    }

    //ImGui::GetCurrentWindowRead();
    // should check whether cursor is hovering either an gameobj or imgui window!!!
    //if (SEIsMouseLeftPressed())
    //{
    //    std::cout << "MOUSE IN \n";
    //    // Return out of function if user is hovering over 
    //    //		anything else other than main game viewport
    //    if (!CheckHoveredViewport())
    //        return;
    //}

    //    for (int i = 0; i < PrefabCreateSwitches.size(); i++)
    //    {
    //        if (PrefabCreateSwitches[i])
    //        {
    //            selPref = i;
    //            break;
    //        }
    //        else
    //        {
    //            selPref = -1;
    //        }
    //    }
    //}

}

/*
*   Returns true if the mouse is currently hovered over viewport
*    - Used for spawning objs/preventing clicking under windows
*/
bool CheckHoveredViewport()
{
    //ImGuiContext* defaultContext = ImGui::GetCurrentContext();

    // Check if mouse is within the application window
    //if (defaultContext->HoveredWindow == nullptr)
    //if(ImGui::IsWindowHovered)
    //    return false;

    // Get hovered Imgui window name
    std::string hoveredWindName{};
    auto hoveredWindow = ImGui::GetCurrentContext()->HoveredWindow;
    if (hoveredWindow)
        hoveredWindName = hoveredWindow->Name;

    // I want to be on viewport
    //size_t foundPos = hoveredWindName.find("GameWindow");
#pragma warning(disable: 4551)
    if (ImGui::IsWindowHovered/*(ImGuiHoveredFlags_AllowWhenBlockedByPopup)*/)
    {
#ifdef _DEBUG
        //std::cout << hoveredWindName << std::endl;// "I am currently on Game Window!\n";
#endif // _DEBUG

        if (hoveredWindName == "ViewPort")
            return true;
    }
    else
    {

#ifdef _DEBUG
        std::cout << "I am NOTONOTNONTO on Game Window!\n";
#endif // _DEBUG


        return false;
    }
    return false;
}

// Standard functions
void SE_UIUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    SE_DockSpace();

    // Sub windows 
    SE_NotifWindow();
    SE_SetTextureWindow();
    SE_UIAddComponentsWindow();
    SE_DialogueManagerWindow();
    SE_RenameObjects();

    HandleMouseState();
}
void SE_UIRender()
{
    SE_Viewport();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void SE_UIExit()
{
    // Serialise container before killing
    SESerialiseJSON(prefab_container_go, PREFAB_FILE);

    // Delete prefab container
    for (const auto& obj : prefab_container_go)
    {
        if (obj)
            delete obj;
    }

    /*for (const auto& obj : prefab_container_backgnd)
    {
        if (obj)
            delete obj;
    }

    for (const auto& obj : prefab_container_foregnd)
    {
        if (obj)
            delete obj;
    }

    for (const auto& obj : prefab_container_gameply)
    {
        if (obj)
            delete obj;
    }

    for (const auto& obj : prefab_container_overlay)
    {
        if (obj)
            delete obj;
    }

    for (const auto& obj : prefab_container_uielems)
    {
        if (obj)
            delete obj;
    }*/


    // kill all raw pointers
    if (spriteCursor)
        delete spriteCursor;

    if (diaNodeACursor)
        delete diaNodeACursor;

    if (diaNodeBCursor)
        delete diaNodeBCursor;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


//void SE_UIDrawLevelSelection()
//{
//    ImGui::SetNextWindowSize(ImVec2(200, 100));
//    ImGui::Begin("Level Menu", 0, ImGuiWindowFlags_NoResize);
//    
//
//    if (ImGui::BeginCombo("##levellist ", previewLevelName.c_str()))
//    {
//
//        for (int i = 0; i < LevelList.size(); i++)
//        {
//            // Populate the list with selectable levels
//            ImGui::Selectable(LevelList[i], &levelCursor[i]);     
//            if (levelCursor[i])
//            {
//                // Display selected string
//                previewLevelName = LevelList[i];
//                levelCursor[i] = true;
//                
//                // Deselect any non selected options
//                for (int j = 0; j < LevelList.size(); j++)
//                {
//                    if (j != i)
//                        levelCursor[j] = false;
//                }
//                
//            }
//        }
//
//        ImGui::EndCombo();
//
//        
//    }
//   
//
//    ImGui::End();
//}

// Main ImGui Windows
void SE_UIDrawMainBar()
{
    //ImGui::Begin("StormEngine");
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open"))
            {
                // fileWind_open = true;// !fileWind_open;
                std::string in = SEOpenFile("json (*.json)\0*.json\0", SEGetGLWindow());

                // data give me
                if (!in.empty())
                {
                    current_GO = nullptr;
                    current_prefab_GO = nullptr;
                    SEClearScene(); //
                    SEDeserialiseJSON(in);
                }

                // sort data via ML?

            }
            if (ImGui::MenuItem("Save"))
            {
                // open windows Window
                std::string out = SESaveFile("json (*.json)\0*.json\0", SEGetGLWindow());
                if (!out.empty())
                {
                    // check if the file has .json extension and add it if not
                    if (out.find(".json") == std::string::npos)
                        out += ".json";

                    // Serialise data
                    auto current_game_objects = SEGetAllLevels();
                    SESerialiseJSON(current_game_objects, out);
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Clear Scene")) {
                SEClearScene();
                current_GO = nullptr;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Preferences")) {
                settings_open = true;// !settings_open;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    //ImGui::Button("Test!");
    //ImGui::End();
}
void SE_UIDrawFileManager()
{
    if (!fileWind_open)
        return;

    if (ImGui::Begin("File Manager", &fileWind_open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::SetCursorPos(ImVec2(70, 60));
        ImGui::Text("Current scene");

        ImGui::SetCursorPos(ImVec2(190, 55));
        if (ImGui::Button("Choose File", ImVec2(220, 30)))
        {
            //OpenScene();
        }

        ImGui::SetCursorPos(ImVec2(70, 110));
        ImGui::Text("Next scene");

        ImGui::SetCursorPos(ImVec2(190, 105));
        if (ImGui::Button("Choose File", ImVec2(220, 30)))
        {
            // Object?
        }

        ImGui::SetCursorPos(ImVec2(30, 175));
        if (ImGui::Button("New Scene", ImVec2(130, 30)))
        {
            //NewScene();
        }

        ImGui::SetCursorPos(ImVec2(190, 175));
        if (ImGui::Button("Save Scene", ImVec2(130, 30)))
        {

        }

        ImGui::SetCursorPos(ImVec2(350, 175));
        if (ImGui::Button("Save Scene As...", ImVec2(130, 30)))
        {
            // Object?
        }
    }ImGui::End();
}
void SE_UIDrawSettingsWindow()
{
    if (!settings_open)
        return;

    bool req1 = false; //bgm_isPlaying                      TODO

    if (ImGui::Begin("Settings", &settings_open, ImGuiWindowFlags_NoCollapse))
    {
        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Game Settings"))
            {
                //ImGui::SetCursorPos(ImVec2(10, 30));
                ImGui::Text("Set BGM On : ");
                //ImGui::SetCursorPos(ImVec2(100, 25));
                ImGui::SameLine();
                ImGui::Checkbox("      ", &req1);           // TODO

               /* if (!req1) {
                    ImGui::BeginDisabled();
                }*/

                //ImGui::SameLine();
                //ImGui::Checkbox("##bgmcheck", &bgm_isPlaying);
                //static std::string previewComponent = "";
                if (ImGui::BeginCombo("Select BGM", previewComponent.c_str()))
                {
                    auto BGMMap = SEGetAudioBGMResources();     // Get all BGM audio files
                    std::vector<std::string> BGMVector;         // Create a temp storage to hold all BGM audio file names
                    for (const auto& bgm : BGMMap)
                    {
                        BGMVector.push_back(bgm.first);         // Fill tmp storage with BGM audio file names
                    }
                    size_t BGMSize = BGMVector.size();          // Get size of BGM audio files

                    std::vector<bool> bgmCursor;                // Create tmp storage to hold cursor of current BGM selected
                    for (size_t i = 0; i < BGMSize; ++i)
                    {
                        bgmCursor.push_back(1);                 // Fill tmp storage with true
                    }

                    for (size_t i = 0; i < BGMSize; i++)
                    {
                        // Populate the list with selectable components
                        std::string bgmList = BGMVector[i];
                        if (ImGui::Selectable(bgmList.c_str()))
                        {
                            SEPlaySound(AUDIOTYPE::BGM, bgmList);

                            // if selected component is already selected
                            if (previewComponent == BGMVector[i])
                                break;

                            if (bgmCursor[i])	// selected component
                            {
                                // Display selected componnet's respective string
                                previewComponent = BGMVector[i];

                                // Deselect any non-selected options
                                for (int j = 0; j < BGMSize; j++)
                                {
                                    if (j != i)
                                    {
                                        // TODO: Ray -> SEStopSound(bgmList);
                                        bgmCursor[j] = false;
                                    }
                                }
                            }
                        }
                    }

                    /*auto BGMContainer = SEGetAudioBGMResources();
                    for (const auto& bgm : BGMContainer)
                    {
                        auto bgmName = bgm.first.c_str();
                        if (ImGui::Selectable(bgmName))
                        {

                        }
                    }*/

                    ImGui::EndCombo();
                }
                int bgmVol = 50; //bgm_vol
                ImGui::SliderInt("BGM Vol", &bgmVol, 0, 100);

                /*if (!bgm_isPlaying) {
                    ImGui::EndDisabled();
                }
                ImGui::EndDisabled();*/



                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Editor Settings"))
            {
                //Thm_Mgr->Config_ThemeButton();
                //ImGui::Text("Add editor stuff here!");
                //ImGui::Text("Hide Grid  : ");
                //ImGui::Checkbox("##check", &grid);
                //ImGui::Text("Set Editor Colors  : ");
                //ImGui::Text("Set Editor Rounded edges  : ");
                //ImGui::Text("Set Editor blah blah blah lol");

                ImGui::SliderInt("Prefab per Column", &buttons_perColumn, 1, 10);

                ImGui::EndTabItem();

                const char* items[] = { "1600x900", "1920x1080", "2560x1440", "3840x2160" };
                static int item_current = 0;
                ImGui::Combo("Window Size", &item_current, items, IM_ARRAYSIZE(items));
                ImGui::SameLine(); HelpMarker(
                    "Sets the current Editor to the specified window size.");

                switch (item_current)
                {
                case 0:
                    SESetWindowWidthHeight(1600, 900);
                    break;
                case 1:
                    SESetWindowWidthHeight(1920, 1080);
                    break;
                case 2:
                    SESetWindowWidthHeight(2560, 1440);
                    break;
                case 3:
                    SESetWindowWidthHeight(3860, 2160);
                    break;
                default:
                    assert("invalid screen AR!\n");
                    break;
                }

            }
            ImGui::EndTabBar();
        }
    }ImGui::End();
}
void SE_UIDrawLayerRender()
{


    if (ImGui::Begin("Render Layers"/*, NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
                                                | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
                                                | ImGuiWindowFlags_NoBringToFrontOnFocus*/))
    {
        ImGui::SameLine();
        if (ImGui::Button("Overlay", ImVec2(80, 30)))
        {
            //Insert function here
            currRender = RENDERLAYER_STATE::RL_OVERLAY;
            rendLayerString = "Overlay";
        }

        ImGui::SameLine();
        if (ImGui::Button("UI Elements", ImVec2(130, 30)))
        {
            //Insert function here
            currRender = RENDERLAYER_STATE::RL_UIELEMENTS;
            rendLayerString = "UI Elements";
        }

        ImGui::SameLine();
        if (ImGui::Button("Foreground", ImVec2(110, 30)))
        {
            //Insert function here
            currRender = RENDERLAYER_STATE::RL_FOREGROUND;
            rendLayerString = "Foreground";
        }

        ImGui::SameLine();
        if (ImGui::Button("Gameplay", ImVec2(100, 30)))
        {
            //Insert function here
            currRender = RENDERLAYER_STATE::RL_GAMEPLAY;
            rendLayerString = "Gameplay";

            //??? lol
            SELayerStackManager->SEFindLayer("Gameplay")->GetLayerObjects();

            //for(GameObject* )
        }

        //ImGui::SameLine();
        //if (ImGui::Button("Background Objects", ImVec2(160, 30)))
        //{
        //    //Insert function here
        //    currRender = RENDERLAYER_STATE::RL_BACKGROUND_OBJ;
        //    rendLayerString = "Background Objects";
        //}

        ImGui::SameLine();
        if (ImGui::Button("Background", ImVec2(110, 30)))
        {
            //Insert function here
            currRender = RENDERLAYER_STATE::RL_BACKGROUND;
            rendLayerString = "Background";
        }


        ImGui::Text("Current Render Mode : ");
        ImGui::SameLine();
        ImGui::Text(rendLayerString.c_str());

        if (currMouse == MOUSE_STATE::M_CREATING)
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "CREATING OBJECT : ");
            ImGui::SameLine();
            ImGui::Text(current_prefab_GO->getName().c_str());
            ImGui::SameLine();
            if (ImGui::Button("Stop Creating"))
            {
                for (int i = 0; i < PrefabCreateSwitches.size(); ++i)
                {
                    PrefabCreateSwitches[i] = false;
                }
                currMouse = MOUSE_STATE::M_IDLE;
            }
        }
        if (currMouse == MOUSE_STATE::M_SELECT)
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 1.0f, 1.0f), "SELECTING OBJECT : ");
            ImGui::SameLine();
            if (current_GO)
            {
                ImGui::Text(current_GO->getName().c_str());
                ImGui::SameLine();
                if (ImGui::Button("Stop Selecting"))
                {
                    current_GO = nullptr;
                    currMouse = MOUSE_STATE::M_IDLE;
                }
            }
        }


    }ImGui::End();
}
void SE_UIDrawHierarchy()
{
    if (ImGui::Begin("Hierarchy"))
    {
        if (ImGui::Button("Delete Object"))
        {

            if (current_GO)
            {
                // [Deletes gameObject from Layer's database]
                Texture* ptex = (Texture*)current_GO->getComponent(CT_TEXTURE);

                if (ptex)
                    SELayerStackManager->SEFindLayer(ptex->GetCurrentRenderLayer())->DeleteLayerObject(current_GO->getName());

                // [Deletes gameObject from goArray]
                SEDeleteObjectByName<LEVEL::LEVEL_1>(current_GO->getName());


                // Must close insp. If insp open, it will try to access the components of th deleted objs
                inspector_open = !inspector_open;
                current_GO = nullptr;
            }

        }


        // For Filtering display of specific layers
        ImGui::SameLine();
        if (!hier_showall)
        {
            if (ImGui::Button("Show Layer Objects"))//, ImVec2(80, 30)))
            {
                hier_showall = true;

            }
            ImGui::Text("Currently Displaying :");
            ImGui::SameLine();
            ImGui::Text("All Objects");

        }
        else
        {
            if (ImGui::Button("Show All Objects"))//, ImVec2(80, 30)))
            {
                hier_showall = false;

            }
            ImGui::Text("Currently Displaying :");
            ImGui::SameLine();
            ImGui::Text(rendLayerString.c_str());
        }

        static bool flagFound{};
        static std::string result{};
        static char searchObject[128] = "hehe";
        ImGui::Text("Search GameObject");
        ImGui::InputTextWithHint("##", "lala45", searchObject, 128);
        auto g = SEFindObjectByName(searchObject);
        flagFound = g ? 1 : 0;

        if (flagFound)
        {
            result = "Found!";
            ImGui::TextColored(ImVec4(0, 1, 0, 1), result.c_str());     // green 
            if(g) current_GO = g;
        }
        else
        {
            result = "Not Found!";
            ImGui::TextColored(ImVec4(1, 0, 0, 1), result.c_str());     // red
        }
        /************************************************
        *          HIERARCHY LIST BOX
        ************************************************/
        ImVec2 listboxsize = ImVec2(250, 390);
        if (ImGui::ListBoxHeader("##hierarchyList", listboxsize))
        {

            //std::unordered_multimap<std::string, GameObject*> FullContainer = SEFindObjectsByLevel(LEVEL::LEVEL_1);      // Get Game Objects in Level 0

            // Contrary to this bool name, this if statement checks to display individual layer objects (For displaying all, check "else" counterpart)
            if (hier_showall)
            {


                // Go thru all layers
                for (auto layer_go : SELayerStackManager->m_layer_stack)
                {

                    // Check if the layer in loop is the layer we selected
                    // "rendLayerString" is set in SE_UIDrawLayerRender() everytime the render layer button is pressed
                    if (layer_go->GetLayerName() == rendLayerString)
                    {
                        //std::cout << "Current Layer name : " << layer_go->GetLayerName() << "\n";
                        //std::cout << "Total Layer obj amt : " << layer_go->GetLayerObjects().size() << "\n";
                        // if layer is the one we want, list all the objs in that layer
                        for (auto go : layer_go->GetLayerObjects())
                        {
                            Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);

                            if (pTexture)
                            {
                                if (ImGui::Selectable(go->getName().c_str(), &inspector_open))
                                {
                                    current_GO = go;

                                    inspector_open = true;
                                }
                            }
                            //std::cout << go->getName() << ", " << pTexture->m_priority_level << std::endl;
                        }
                    }

                }

            }
            else
            {
                // Go thru entire game obj containers
                //for (std::pair <const std::string, GameObject*> obj : SEFindObjectsByLevel(currentLevelLoaded))
                //{
                //    std::cout << "Object in level : " << obj.second->getName() << "\n";
                // Go thru all layers
                for (auto layer_go : SELayerStackManager->m_layer_stack)
                {
                    for (auto go : layer_go->GetLayerObjects())
                    {
                        if (go)//(obj.second)
                        {
                            if (ImGui::Selectable(go->getName().c_str(), &inspector_open))//(obj.second->getName().c_str(), &inspector_open))
                            {
                                current_GO = go;//obj.second;

                                inspector_open = true;
                            }
                        }
                    }
                }
            }
            //for (auto obj : Factory::gameObjectArray)
            //{
                //if (ImGui::Selectable("TODO : Populate GO"))    //obj.first.c_str()
                //{
                //    // Inspector remains open if selected diff obj
                //    // Inspector closes if same obj selected
                //    /*if (current_GO == obj.second)
                //    {
                //        if (inspector_open)
                //            inspector_open = false;
                //        else
                //        {
                //            currMouse = Systems_::MOUSE_STATE::M_SELECTING;
                //            inspector_open = true;
                //        }
                //    }
                //    else
                //    {
                //        currMouse = Systems_::MOUSE_STATE::M_SELECTING;
                //        inspector_open = true;
                //    }*/
                //
                //    inspector_open = !inspector_open;
                //
                //    //is_hierSelected = true;
                //    //current_GO = obj.second;
                //}
                //ImGui::Text(obj.first.c_str());
                /*
                Function(obj) :
                 - loop thru all the components in the obj
                 - list of bool. each bool represents a component
                */

                //}
            ImGui::ListBoxFooter();
        }





        //ImGui::Selectable("Not Selected", false);



    }ImGui::End();

    // Go thru entire game obj containers
    /*for (std::pair <const std::string, GameObject*> obj : SEFindObjectsByLevel(currentLevelLoaded))
    {
        if (obj.second)
        {
            std::cout << "OBJs in level : " << obj.second->getName() << "\n";
        }
        else
        {
            std::cout << "null obj found!\n";
        }
    }*/
}
void SE_UIDrawPalette()
{
    ImGui::Begin("Palette");
    if (ImGui::Button("Create Prefab"))
    {
        addCompoWind_open = false;
        createPrefab_open = true;

        // Reset all cursor bools 
        for (int i = 0; i < sizeof(componentCursor); i++)
            componentCursor[i] = false;
    }
    // Delete Selected Prefabs (Only when any prefabs are selected)
    ImGui::SameLine();
    for (int i = 0; i < PrefabCreateSwitches.size(); i++)
    {
        if (PrefabCreateSwitches[i])
        {
            if (ImGui::Button("Delete Selected Prefab"))
            {
                PrefabCreateSwitches[i] = false;


                PrefabCreateSwitches.erase(PrefabCreateSwitches.begin() + i);

                // Added by rommel 29/1/23
                // delete allocated mem for GO
                if (prefab_container_go[i])
                    delete prefab_container_go[i];

                /*if (currRender == RENDERLAYER_STATE::RL_GAMEPLAY)
                {
                    if (prefab_container_gameply[i])
                        delete prefab_container_gameply[i];
                }
                if (currRender == RENDERLAYER_STATE::RL_BACKGROUND)
                {
                    if (prefab_container_backgnd[i])
                        delete prefab_container_backgnd[i];
                }
                if (currRender == RENDERLAYER_STATE::RL_FOREGROUND)
                {
                    if (prefab_container_foregnd[i])
                        delete prefab_container_foregnd[i];
                }
                if (currRender == RENDERLAYER_STATE::RL_OVERLAY)
                {
                    if (prefab_container_overlay[i])
                        delete prefab_container_overlay[i];
                }
                if (currRender == RENDERLAYER_STATE::RL_UIELEMENTS)
                {
                    if (prefab_container_uielems[i])
                        delete prefab_container_uielems[i];
                }*/


                prefab_container_go.erase(prefab_container_go.begin() + i);
                /*prefab_container_uielems.erase(prefab_container_uielems.begin() + i);
                prefab_container_overlay.erase(prefab_container_overlay.begin() + i);
                prefab_container_gameply.erase(prefab_container_gameply.begin() + i);
                prefab_container_backgnd.erase(prefab_container_backgnd.begin() + i);
                prefab_container_foregnd.erase(prefab_container_foregnd.begin() + i);*/


                // Assign current go to nullptr to prevent spawning and accessing invlaid addr space.
                current_prefab_GO = nullptr;
                //PrefabsContainer.erase(PrefabsContainer.begin() + i);

                currMouse = MOUSE_STATE::M_IDLE;

            }
        }
    }


    // Text Display showing what prefab is currently selected
    std::string prefabStatus = "";

    // If there is no prefabs created "at all"
    if (PrefabCreateSwitches.empty())
    {
        prefabStatus = "No Prefabs";
    }
    else
    {
        for (int i = 0; i < PrefabCreateSwitches.size(); i++)
        {
            // Show Selected Prefabs
            if (PrefabCreateSwitches[i])
            {
                prefabStatus = "Selected : ";
                ImGui::Text(prefabStatus.c_str());
                ImGui::SameLine();
                //ImGui::Text(PrefabsContainer[i].c_str());


                ImGui::Text(prefab_container_go[i]->m_name.c_str());
                //ImGui::Text(PrefabsContainer[i].c_str());
            }
            else
                prefabStatus = "No Prefabs Selected";
        }
    }



    // Arranging the buttons for each prefabs
    float buttonCursorXPOS = 10.f;
    float buttonCursorYPOS = 80.f;
    float buttonXPadding = 5.f;
    float buttonYPadding = 5.f;

    //int numbOfButtons = PrefabsContainer.size(); 
    auto numbOfButtons = prefab_container_go.size();

    /*if (currRender == RENDERLAYER_STATE::RL_GAMEPLAY)
    {
        numbOfButtons = prefab_container_gameply.size();
    }
    if (currRender == RENDERLAYER_STATE::RL_BACKGROUND)
    {
        numbOfButtons = prefab_container_backgnd.size();
    }
    if (currRender == RENDERLAYER_STATE::RL_FOREGROUND)
    {
        numbOfButtons = prefab_container_foregnd.size();
    }
    if (currRender == RENDERLAYER_STATE::RL_OVERLAY)
    {
        numbOfButtons = prefab_container_overlay.size();
    }
    if (currRender == RENDERLAYER_STATE::RL_UIELEMENTS)
    {
        numbOfButtons = prefab_container_uielems.size();
    }*/




    for (int i = 0; i < numbOfButtons; i++)
    {
        // Automatically Repositions cursor for button creation
        if ((i % buttons_perColumn == 0) && i != 0)
        {
            // Reset Button X pos when number of buttons in rows reached
            buttonCursorXPOS = 10.f;
            // Change Button Y pos when number of buttons in rows reached
            buttonCursorYPOS += (ButtonSize.y + buttonYPadding);
        }

        ImGui::SetCursorPos(ImVec2(buttonCursorXPOS, buttonCursorYPOS));



        // This is the size of the container holding bools 
        //    to check which button is pressed respectively 
        auto listOfButtonSwitches = PrefabCreateSwitches.size();

        std::string ButtonName = prefab_container_go[i]->m_name.c_str();
        //std::string ButtonName = PrefabsContainer[i].c_str();

        /*if (currRender == RENDERLAYER_STATE::RL_GAMEPLAY)
        {
            ButtonName = prefab_container_gameply[i]->m_name.c_str();
        }
        if (currRender == RENDERLAYER_STATE::RL_BACKGROUND)
        {
            ButtonName = prefab_container_backgnd[i]->m_name.c_str();
        }
        if (currRender == RENDERLAYER_STATE::RL_FOREGROUND)
        {
            ButtonName = prefab_container_foregnd[i]->m_name.c_str();
        }
        if (currRender == RENDERLAYER_STATE::RL_OVERLAY)
        {
            ButtonName = prefab_container_overlay[i]->m_name.c_str();
        }
        if (currRender == RENDERLAYER_STATE::RL_UIELEMENTS)
        {
            ButtonName = prefab_container_uielems[i]->m_name.c_str();
        }*/


        /************************************************
        *          Create Prefab Button
        ************************************************/
        if (ImGui::Button(ButtonName.c_str(), ButtonSize))
        {
            //std::cout << "i val : " << i << std::endl;
            //std::cout << "j val : " << listOfButtonSwitches << std::endl;
            for (int j = 0; j < listOfButtonSwitches; ++j)
            {

                // Check if button selected is button that is on
                if (j == i)
                {
                    // Toggle selection
                    PrefabCreateSwitches[j] = !PrefabCreateSwitches[j];

                    // Set mouse state if ANY button is creating          
                    SEGetCursorX();
                    if (PrefabCreateSwitches[j])
                    {
                        currMouse = MOUSE_STATE::M_CREATING;
                        for (const auto& obj : prefab_container_go)
                        {
                            if (obj)
                            {
                                if (obj->m_name == prefab_container_go[j]->m_name)
                                {
                                    current_prefab_GO = obj;

                                }
                                /*else
                                    current_prefab_GO = nullptr;*/
                            }
                        }
                    }
                    else
                        currMouse = MOUSE_STATE::M_IDLE;
                }
                else
                {
                    PrefabCreateSwitches[j] = false;
                }

            }


        }


        buttonCursorXPOS += (ButtonSize.x + buttonXPadding);
    }


    ImGui::End();
}
void SE_UIDrawObjInspector()
{
    if (!inspector_open)
        return;

    if (ImGui::Begin("Inspector", &inspector_open))
    {
        ImGui::Separator();
        ImGui::Text("Object Name : ");
        if (current_GO)
        {
            ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), current_GO->getName().c_str());
        }
        ImGui::SameLine();
        if (ImGui::Button("Rename Object"))
        {
            renameObject_open = true;
        }
        ImGui::Separator();

        ImVec2 HeaderButtonSize = ImVec2(100, 30);
        if (ImGui::Button("Add COMPONENT", HeaderButtonSize))
        {
            // TODO: get the selected component from imgui
            //current_GO->addComponent(COMPONENT_TYPE::CT_TRANSFORM);
            addCompoWind_open = true;
            createPrefab_open = false;

            // Reset all cursor bools 
            for (int i = 0; i < sizeof(componentCursor); i++)
                componentCursor[i] = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete OBJECT", HeaderButtonSize))
        {
            if (current_GO)
            {

                // [Deletes gameObject from Layer's database]
                Texture* ptex = (Texture*)current_GO->getComponent(CT_TEXTURE);

                if (ptex)
                    SELayerStackManager->SEFindLayer(ptex->GetCurrentRenderLayer())->DeleteLayerObject(current_GO->getName());

                // [Deletes gameObject from goArray]
                SEDeleteObjectByName<LEVEL::LEVEL_1>(current_GO->getName());


                // Must close insp. If insp open, it will try to access the components of th deleted objs
                inspector_open = !inspector_open;
                current_GO = nullptr;
            }
        }
        inspector_yPos += 80;

        if (current_GO)
        {
            transformSection_active = true;
            COMP_TransformSection(ImVec2(0, inspector_yPos), true);

            collisionSection_active = true;
            COMP_CollisionSection(ImVec2(0, inspector_yPos), true);

            SpriteSection_active = true;
            COMP_SpriteSection(ImVec2(0, inspector_yPos), true);

            // TODO : ButtonGui Component
            //ButtonGUISection_active = true;
            //COMP_ButtonGUISection(ImVec2(0, inspector_yPos), true);

            DialogueSection_active = true;
            COMP_DialogueSection(ImVec2(0, inspector_yPos), true);

            //TextCompSection_active = true;
            //COMP_TextSection(ImVec2(0, inspector_yPos), true);

            //WaypointsSection_active = true;
            //COMP_WaypointsSection(ImVec2(0, inspector_yPos), true);

        }


        // reset y position (Other objects using same imgui window)
        ImGui::Separator();
        inspector_yPos = 30;

    }ImGui::End();
}
void SE_UIDrawCreatePrefab()
{
    if (!createPrefab_open)
        return;

    // Window Headers
    ImGui::SetNextWindowSize(ImVec2(340, 400));
    ImGui::Begin("Create New Prefab!", &createPrefab_open, ImGuiWindowFlags_NoResize);

    ImGui::Separator();
    ImGui::TextColored(ImVec4(.5f, 0.8f, 1.0f, 1.0f), "New Prefab");
    ImGui::Separator();

    ImGui::Text("Name : ");
    ImGui::SameLine();
    ImGui::InputText("##newPrefabName", newPrefabName, 64);     // Last param is the size of char array given

    ImGui::Text("Add Components : ");
    ImGui::SameLine();

    // Drop Down Menu for adding components
    ImGui::PushItemWidth(150.f);
    if (ImGui::BeginCombo("  ", previewComponent.c_str()))
    {
        previewComponent = "";
        for (int i = 0; i < componentList.size(); i++)
        {
            // Populate the list with selectable components
            // ComponentCursor is a list of bools to tell us which component will be added to prefab when created to serialize
            ImGui::Selectable(componentList[i], &componentCursor[i]);
            //if (componentList[i] == "Transform")
            if (strcmp(componentList[i], "Transform") == 0)
            {
                componentCursor[i] = true;      // Automatically sets Transform to true cuz everything need position

            }

            if (componentCursor[i])
            {
                // Display selected string
                previewComponent = componentList[i];
                componentCursor[i] = true;

            }
        }

        ImGui::EndCombo();
        ImGui::PopItemWidth();
    }


    // Various checks for special components with extra needs
    /*
    * For Transform : Obj should spawn at mouse position, so no need to init pos
    *               : Scale and rotate should have some default values (eg scale = (10,10))
    */
    static bool check_textureComp = false;
    for (int i = 0; i < componentList.size(); i++)
    {
        //if (componentList[i] == "Transform")
        if (strcmp(componentList[i], "Transform") == 0)
        {
            componentCursor[i] = true;      // Always set Transform component to be true so it can be added 
        }
        //if (componentList[i] == "Texture")
        if (strcmp(componentList[i], "Texture") == 0)
        {
            // For checking if the texture component exist. (For "Set texture" button to awake)
            if (componentCursor[i])
                check_textureComp = true;
        }

    }




    // Box list : To show which components will be added to prefab when created
    if (ImGui::ListBoxHeader("##createPrefab", ImVec2(150, 200)))
    {
        for (int i = 0; i < sizeof(componentCursor); i++)
        {
            if (componentCursor[i])
            {
                // deleteCompCursor tells Remove Component Button later which components to remove from list (Exclude when creating prefabs)
                ImGui::Selectable(componentList[i], &deleteCompCursor[i]);
            }
        }

        ImGui::ListBoxFooter();
    }


    // Remove Component Button
    if (ImGui::Button("Remove Component"))
    {
        for (int i = 0; i < sizeof(deleteCompCursor); i++)
        {
            if (deleteCompCursor[i])
            {
                //if (componentList[i] == "Transform")
                if (strcmp(componentList[i], "Transform") == 0)
                {
                    NotifWindowSize = ImVec2(500, 100);
                    NotifWindowPos = ImVec2(500, 400);
                    NotifWindowMessage = "Cannot delete transform! \n(What, you want something to exist without positions? :P)";
                    notifWindow_open = true;
                    deleteCompCursor[i] = false;
                }
                //else if (componentList[i] == "Texture")
                else if (strcmp(componentList[i], "Texture") == 0)
                {
                    check_textureComp = false;      // turn off set texture button when Texture component is no longer included
                    deleteCompCursor[i] = false;
                    componentCursor[i] = false;
                }
                else
                {
                    deleteCompCursor[i] = false;
                    componentCursor[i] = false;
                }
            }
        }
    }

    if (check_textureComp)
    {
        ImGui::SameLine();
        if (ImGui::Button("Set Texture"))
        {
            setTextureWindow_open = true;
        }
    }




    /************************************************
    *          CREATE PREFAB BUTTON
    ************************************************/
    ImGui::Separator();
    static bool prefabNameExist = false;
    if (ImGui::Button("CREATE PREFAB", ImVec2(150, 50)))
    {
        // Serialize Here
        /*
        * Values Available :
        *  - Loop Thru "componentCursor"
        *  - each "true" element means create object with that component
        */

        // Create object

        // loop "componentCursor"
        // if [i], add component to object?


        // save object to json
        // delete object

        // Reset all cursor bools 
        /*for (int i = 0; i < sizeof(componentCursor); i++)
        {
            componentCursor[i] = false;
        }*/

        // check if prefab name exist
        for (int i = 0; i < prefab_container_go.size(); ++i)
        {
            if (prefab_container_go[i]->getName() == newPrefabName)
            {
                NotifWindowMessage = "Name of prefab exist! \n Please use another name thanks!";
                NotifWindowSize = ImVec2(500, 200);
                NotifWindowPos = ImVec2(500, 300);
                notifWindow_open = true;
                prefabNameExist = true;
            }
        }

        if (!prefabNameExist)
        {
            std::unique_ptr<GameObject> newGoPrefab = std::make_unique<GameObject>(newPrefabName);
            if (componentCursor[0]) newGoPrefab->addComponent(CT_TRANSFORM);
            if (componentCursor[1]) newGoPrefab->addComponent(CT_COLLISION);
            if (componentCursor[2])
            {
                newGoPrefab->addComponent(CT_TEXTURE);
                Texture* text = (Texture*)newGoPrefab->getComponent(CT_TEXTURE);
                if (text)
                {
                    std::filesystem::path tmp = previewCreateNewPrefabSprite;
                    if (tmp.string().empty())
                        tmp = "./Assets/Textures/Character_MainCharacter_Sprite.png";
                    text->AnimationTextures[ACTION::ACTION_NULL].emplace(tmp.filename().string(), tmp);
                }

            }
            prefab_container_go.push_back(newGoPrefab.release());
            PrefabCreateSwitches.push_back(false);

            /*if (currRender == RENDERLAYER_STATE::RL_GAMEPLAY)
            {
                prefab_container_gameply.push_back(newGoPrefab.release());
            }
            if (currRender == RENDERLAYER_STATE::RL_BACKGROUND)
            {
                prefab_container_backgnd.push_back(newGoPrefab.release());
            }
            if (currRender == RENDERLAYER_STATE::RL_FOREGROUND)
            {
                prefab_container_foregnd.push_back(newGoPrefab.release());
            }
            if (currRender == RENDERLAYER_STATE::RL_OVERLAY)
            {
                prefab_container_overlay.push_back(newGoPrefab.release());
            }
            if (currRender == RENDERLAYER_STATE::RL_UIELEMENTS)
            {
                prefab_container_uielems.push_back(newGoPrefab.release());
            }*/

            // Close CreatePrefab window after creating
            createPrefab_open = false;
        }
        else
        {
            prefabNameExist = false;

        }
    }


    ImGui::End();
}

// Sub Windows (Appears when certain functions are required)
void SE_NotifWindow()
{
    if (!notifWindow_open)
        return;

    ImGui::SetNextWindowSize(NotifWindowSize);
    ImGui::SetNextWindowPos(NotifWindowPos);
    ImGui::Begin("Notice", &notifWindow_open);

    ImGui::Text(NotifWindowMessage.c_str());

    if (ImGui::Button("Okay! Noted!"))
    {
        NotifWindowMessage = "";
        notifWindow_open = false;
    }

    ImGui::End();
}
void SE_UIAddComponentsWindow()
{
    if (!addCompoWind_open)
        return;

    // Only allowed to add components when there is an object selected
    if (current_GO == nullptr)
    {
        addCompoWind_open = false;
        return;
    }

    // Window Headers
    ImGui::SetNextWindowSize(ImVec2(340, 400));
    ImGui::Begin("Additional Components", &addCompoWind_open, ImGuiWindowFlags_NoResize);

    ImGui::Separator();
    ImGui::TextColored(ImVec4(.5f, 0.8f, 1.0f, 1.0f), "Adding Components");
    ImGui::Separator();

    ImGui::Text("Add Components : ");
    ImGui::SameLine();


    // Drop Down Menu for adding components
    ImGui::PushItemWidth(150.f);
    if (ImGui::BeginCombo("##addCompoList", previewComponent.c_str()))
    {

        previewComponent = "";
        for (int i = 0; i < componentList.size(); i++)
        {
            // Populate the list with selectable components
            // ComponentCursor is a list of bools to tell us which component will be added to prefab when created to serialize
            ImGui::Selectable(componentList[i], &componentCursor[i]);
            //if (componentList[i] == "Transform")
            if (strcmp(componentList[i], "Transform") == 0)
            {
                componentCursor[i] = true;      // Automatically sets Transform to true cuz everything need position
            }

            if (componentCursor[i])
            {
                // Display selected string
                previewComponent = componentList[i];
                componentCursor[i] = true;

                //if (componentList[i] == "Collision")
                if (strcmp(componentList[i], "Collision") == 0)
                {
                    current_GO->addComponent(CT_COLLISION);
                }
                //if (componentList[i] == "Texture")
                if (strcmp(componentList[i], "Texture") == 0)
                {
                    current_GO->addComponent(CT_TEXTURE);
                }
                //if (componentList[i] == "Dialogue")
                if (strcmp(componentList[i], "Dialogue") == 0)
                {
                    current_GO->addComponent(CT_DIALOGUE);
                }
                //if (componentList[i] == "ButtonGUI")
                if (strcmp(componentList[i], "ButtonGUI") == 0)
                {

                }

            }
        }

        ImGui::EndCombo();
        ImGui::PopItemWidth();
    }



    // Check and set components that the obj already contains
    for (int i = 0; i < componentList.size(); i++)
    {
        //for(int j = 0; j < current_GO->)
    }



    // Box list : To show which components will be added to prefab when created
    if (ImGui::ListBoxHeader("##createPrefab", ImVec2(150, 200)))
    {
        for (int i = 0; i < sizeof(componentCursor); i++)
        {
            if (componentCursor[i])
            {
                // deleteCompCursor tells Remove Component Button later which components to remove from list (Exclude when creating prefabs)
                ImGui::Selectable(componentList[i], &deleteCompCursor[i]);
            }
        }

        ImGui::ListBoxFooter();
    }


    // Remove Component Button
    if (ImGui::Button("Remove Component"))
    {
        for (int i = 0; i < sizeof(deleteCompCursor); i++)
        {
            if (deleteCompCursor[i])
            {
                //if (componentList[i] == "Transform")
                if (strcmp(componentList[i], "Transform") == 0)
                {
                    NotifWindowSize = ImVec2(500, 100);
                    NotifWindowPos = ImVec2(500, 400);
                    NotifWindowMessage = "Cannot delete transform! \n(What, you want something to exist without positions? :P)";
                    notifWindow_open = true;
                    deleteCompCursor[i] = false;
                }
                else
                {
                    deleteCompCursor[i] = false;
                    componentCursor[i] = false;
                }
            }
        }
    }

    /*if (ImGui::Button("Add Component"))
    {

    }*/







    ImGui::End();
}
void SE_SetTextureWindow()
{
    if (!setTextureWindow_open)
        return;

    ImGui::SetNextWindowSize(ImVec2(600, 100));
    ImGui::SetWindowPos(ImVec2(500, 400));
    ImGui::Begin("Set Texture", &setTextureWindow_open);

    ImGui::Text("Current Sprite : ");

    // drop down menu
    if (ImGui::BeginCombo("  ", previewCreateNewPrefabSprite.c_str()))
    {
        previewCreateNewPrefabSprite = "";

        for (int i = 0; i < spriteList.size(); i++)
        {

            // SpriteCursor is a list of bools to tell us which sprite is being selected
            ImGui::Selectable(spriteList[i].c_str(), &spriteCursor[i]);

            if (spriteCursor[i])
            {
                // Display selected string
                previewCreateNewPrefabSprite = spriteList[i].c_str();

                // Deselect any non selected options
                for (int j = 0; j < spriteList.size(); j++)
                {
                    if (j != i)
                        spriteCursor[j] = false;
                }
            }
        }


        ImGui::EndCombo();
    }

    ImGui::SameLine();
    if (ImGui::Button("Set Texture", ImVec2(100, 30)))
    {

        setTextureWindow_open = false;
    }

    ImGui::End();
}
void SE_DialogueManagerWindow()
{
    if (!dialogueManager_open)
        return;

    // Header 
    ImGui::SetNextWindowSize(ImVec2(500, 650));
    ImGui::SetNextWindowPos(ImVec2(475, 125));
    ImGui::Begin("Dialogue Manager", &dialogueManager_open);
    if (current_GO)
    {
        ImGui::Text("Editing for NPC :"); ImGui::SameLine();
        ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), current_GO->getName().c_str());
    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.5f, 1.0f), "No NPCs Selected!!");
    }


    // Deserialize Dialogue
    DialogueID = SEDialogueManager->dialogue_database.size();


    if (DialogueID == 0)
    {
        /*DialogueID = 1;
        DialoguePos = 1;
        DiaName.push_back("##dialgName"     + std::to_string(DialogueID));
        DiaContent.push_back("##dialgMain"  + std::to_string(DialogueID));
        DiaChoiceA.push_back("##choiceA"    + std::to_string(DialogueID));
        DiaChoiceB.push_back("##choiceB"    + std::to_string(DialogueID));
        DiaJumpA.push_back("##diaJumpA"     + std::to_string(DialogueID));
        DiaJumpB.push_back("##diaJumpB"     + std::to_string(DialogueID));
        SusModA.push_back("##susAmtA"       + std::to_string(DialogueID));
        SusModB.push_back("##susAmtB"       + std::to_string(DialogueID));

        DiaDeleteCursor.push_back("Delete Section " + std::to_string(DialogueID));*/

        ImGui::Text("No Dialogue. Click on the Add Dialogue to start editing.");
    }
    else
    {
        // Initializing Dialogue Data
        DialoguePos = (int)DialogueID;




        //for (SEDialogueNode* node : SEDialogueManager->dialogue_database)
        //{

        for (int i = 0; i < DialoguePos; ++i)
        {
            DiaName.push_back("##dialgName" + std::to_string(i));
            DiaContent.push_back("##dialgMain" + std::to_string(i));
            DiaChoiceA.push_back("##choiceA" + std::to_string(i));
            DiaChoiceB.push_back("##choiceB" + std::to_string(i));
            DiaJumpA.push_back("##diaJumpA" + std::to_string(i));
            DiaJumpB.push_back("##diaJumpB" + std::to_string(i));
            SusModA.push_back("##susAmtA" + std::to_string(i));
            SusModB.push_back("##susAmtB" + std::to_string(i));
            DiaDeleteCursor.push_back("Delete Section " + std::to_string(i));
            DiaSaveBtn.push_back("Save Dialogue " + std::to_string(i));

            DiaPreviewJumpA.push_back("");
            DiaPreviewJumpB.push_back("");

            // Dialogue Name
            static char tmpDiaName[64] = "";
            std::string previous_node_name;

            strcpy_s(tmpDiaName, SEDialogueManager->dialogue_database[i]->SEGetDialogueName().c_str());
            previous_node_name = tmpDiaName;
            ImGui::Separator();
            ImGui::Text("Dialogue Name :"); ImGui::SameLine();
            ImGui::PushItemWidth(choiceBoxSize);
            ImGui::InputText(DiaName[i].c_str(), tmpDiaName, 64); ImGui::SameLine();    // "tmpDiaName" CONTAINS THE ACTUAL DIALOGUE NAME
            ImGui::PopItemWidth();

            // Save Dialogue Name
            SEDialogueManager->SEFindDialogueNode(previous_node_name)->node_name = tmpDiaName;

            // Update GoArray side. 
            for (auto go_pair : goArray)
            {
                GameObject* go = go_pair.second.second;

                Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);

                if (dial)
                {
                    if (dial->parent_node_name == previous_node_name)
                    {
                        dial->parent_node_name = tmpDiaName;
                    }
                }
            }
            /*           GameObject* go = SEFindObjectByName(previous_node_name);
                       go->m_name = tmpDiaName;*/


                       // Delete Button
            if (ImGui::Button(DiaDeleteCursor[i].c_str()))
            {

                DeleteDiaFlag = true;

            }



            // Main Dialogue Box
            static char dialgMain[64] = "";
            // Getting main diag from json
            strcpy_s(dialgMain, SEDialogueManager->dialogue_database[i]->prompt_object->prompt_text.c_str());
            ImGui::Text("Dialogue :");
            ImGui::InputTextMultiline(DiaContent[i].c_str(), dialgMain, IM_ARRAYSIZE(dialgMain),        // "dialgMain" CONTAINS THE ACTUAL DIALOGUE
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 7), ImGuiInputTextFlags_AllowTabInput);   // Last 2 parameters just specifies size of input box

            // Saving main dialogue
            SEDialogueManager->SEFindDialogueNode(tmpDiaName);
            //current_GO->getName();
                //->prompt_object->prompt_text = dialgMain;

            // Update GoArray side
            for (auto go_pair : goArray)
            {
                GameObject* go = go_pair.second.second;

                Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);
                Font* font = (Font*)go->getComponent(CT_FONT);

                if (dial)
                {
                    dial->dialogue_text = dialgMain;
                }

                if (font)
                {
                    font->m_text = dialgMain;
                }
            }

            /*GameObject* go = SEFindObjectByName(current_GO->getName());
            Font* font = (Font*)go->getComponent(CT_FONT);

            if (font)
            {
                font->m_text = dialgMain;
            }*/




            // Choice Boxes
            static char choiceA[64] = "";
            static char choiceB[64] = "";

            std::vector<SEDialogueOption*> option_database;

            // To Check if there is any existing options.
            if (SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions().size() != 0)
            {
                for (int j = 0; j < SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions().size(); ++j)
                {
                    option_database = SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions();
                    strcpy_s(choiceA, option_database[j]->option_text.c_str());
                }
            }
            else
            {
                std::cout << "No Choices/Options! Creating empty...\n";
                SEDialogueManager->dialogue_database[i]->SEAddNewOptionObject(DiaChoiceA[i], choiceA);
                SEDialogueManager->dialogue_database[i]->SEAddNewOptionObject(DiaChoiceB[i], choiceB);
                option_database = SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions();
                strcpy_s(choiceA, option_database[0]->option_text.c_str());
                strcpy_s(choiceB, option_database[1]->option_text.c_str());
            }


            ImGui::Text("Choices :");
            ImGui::InputTextMultiline(DiaChoiceA[i].c_str(), choiceA, IM_ARRAYSIZE(choiceA),
                ImVec2(choiceBoxSize, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_AllowTabInput);      // Last 2 parameters just specifies size of input box
            ImGui::SameLine();
            ImGui::InputTextMultiline(DiaChoiceB[i].c_str(), choiceB, IM_ARRAYSIZE(choiceB),
                ImVec2(choiceBoxSize, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_AllowTabInput);      // Last 2 parameters just specifies size of input box

            // Saving the choices/option texts
            switch (SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions().size())
            {
            case 1:
                SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions()[0]->option_text = choiceA;
                break;
            case 2:
                SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions()[0]->option_text = choiceA;
                SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions()[1]->option_text = choiceB;
                break;
            }






            // Dialogue Jump Input


            ImGui::Text("Dialogue Jump To :");
            /*static char diaJumpA[64] = "";
            static char diaJumpB[64] = "";

            if (SEDialogueManager->dialogue_database[i]->SEGetDialogueOptions().size() != 0)
            {
                strcpy_s(diaJumpA, option_database[0]->next_node_name.c_str());
                strcpy_s(diaJumpB, option_database[1]->next_node_name.c_str());
            }*/

            /*ImGui::PushItemWidth(choiceBoxSize);
            ImGui::InputText(DiaJumpA[i].c_str(), diaJumpA, 64); ImGui::SameLine();
            ImGui::InputText(DiaJumpB[i].c_str(), diaJumpB, 64);
            ImGui::PopItemWidth();*/

            //static std::string previewNodeA = "";
            ImGui::PushItemWidth(choiceBoxSize);
            // Drop Down (JUMP A) ===================================================
            if (ImGui::BeginCombo(DiaJumpA[i].c_str(), DiaPreviewJumpA[i].c_str()))
            {
                // Drop down for Sprite Selection
                for (int k = 0; k < diaNodeList.size(); k++)
                {
                    // nodeCursor is a list of bools to tell us which node is being selected
                    ImGui::Selectable(diaNodeList[k].c_str(), &diaNodeACursor[k]);

                    if (diaNodeACursor[k])
                    {
                        // Display selected string
                        DiaPreviewJumpA[i] = diaNodeList[k].c_str();

                    }
                    // Deselect any non selected options
                    for (int j = 0; j < diaNodeList.size(); j++)
                    {
                        if (j != i)
                            diaNodeACursor[j] = false;
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();

            //static std::string previewNodeB = "";
            // Drop Down (JUMP B) ===================================================
            if (ImGui::BeginCombo(DiaJumpB[i].c_str(), DiaPreviewJumpB[i].c_str()))
            {
                // Drop down for Sprite Selection
                for (int k = 0; k < diaNodeList.size(); k++)
                {
                    // nodeCursor is a list of bools to tell us which node is being selected
                    ImGui::Selectable(diaNodeList[k].c_str(), &diaNodeBCursor[k]);

                    if (diaNodeBCursor[k])
                    {
                        // Display selected string
                        DiaPreviewJumpB[i] = diaNodeList[k].c_str();

                    }
                    // Deselect any non selected options
                    for (int j = 0; j < diaNodeList.size(); j++)
                    {
                        if (j != i)
                            diaNodeBCursor[j] = false;
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::PopItemWidth();

            // Sus modifier
            static int susAmtA = 0;
            static int susAmtB = 0;
            if (SEDialogueManager->dialogue_database[i]->options.size() != 0)
            {
                switch (SEDialogueManager->dialogue_database[i]->options.size())
                {
                case 1:
                    susAmtA = SEDialogueManager->dialogue_database[i]->options[0]->suspicion_value;
                    break;
                case 2:
                    susAmtA = SEDialogueManager->dialogue_database[i]->options[0]->suspicion_value;
                    susAmtB = SEDialogueManager->dialogue_database[i]->options[1]->suspicion_value;
                    break;
                }

            }

            ImGui::Text("Suspicion Modifier :");
            ImGui::Text("Choice 1 :"); ImGui::SameLine();
            ImGui::PushItemWidth(100.f);
            ImGui::InputInt(SusModA[i].c_str(), &susAmtA, 0);
            ImGui::Text("Choice 2 :"); ImGui::SameLine();
            ImGui::InputInt(SusModB[i].c_str(), &susAmtB, 0);
            ImGui::PopItemWidth();




            // Saving Dialogue Datas
            if (ImGui::Button(DiaSaveBtn[i].c_str()))
            {
                // Saving main dialogue
                SEDialogueManager->SEFindDialogueNode(tmpDiaName)->prompt_object->prompt_text = dialgMain;

                GameObject* go = SEFindObjectByName(current_GO->getName());
                Font* font = (Font*)go->getComponent(CT_FONT);

                if (font)
                {
                    font->m_text = dialgMain;
                }
            }




            // END OF DIALOGUE STUFF //
            if (DeleteDiaFlag)
            {
                DiaName.erase(DiaName.begin() + i);
                DiaContent.erase(DiaContent.begin() + i);
                DiaChoiceA.erase(DiaChoiceA.begin() + i);
                DiaChoiceB.erase(DiaChoiceB.begin() + i);
                DiaJumpA.erase(DiaJumpA.begin() + i);
                DiaJumpB.erase(DiaJumpB.begin() + i);
                SusModA.erase(SusModA.begin() + i);
                SusModB.erase(SusModB.begin() + i);

                DiaPreviewJumpA.erase(DiaPreviewJumpA.begin() + i);
                DiaPreviewJumpB.erase(DiaPreviewJumpB.begin() + i);

                DiaDeleteCursor.erase(DiaDeleteCursor.begin() + i);

                --DialoguePos;
                std::cout << "ID : " << DialogueID << "\n";
                std::cout << "Pos : " << DialoguePos << "\n";

                DeleteDiaFlag = false;
            }
        }
        //}

    }


    //static std::vector <std::string> tmpIDContainer{};
    //static std::vector <bool>  tmpIDSwitches{};
    //static int DialgID = 0;
    //
    //if (DialgID == 0)   // init once 
    //{
    //    tmpIDContainer.emplace_back("Add Dialogue " + std::to_string(DialgID)); //id++;
    //    tmpIDSwitches.emplace_back(true);
    //    DialgID = 1;
    //}

    /*if (ImGui::Button(tmpIDContainer[id].c_str()))
    {
        id++;
        tmpIDContainer.push_back("Add Dialogue " + std::to_string(id));
        tmpIDSwitches.push_back(true);
    }*/

    /*for (int i = 0; i < tmpIDSwitches.size(); ++i)
    {
        if (ImGui::Button(tmpIDContainer[i].c_str()))
        {
            tmpIDContainer.emplace_back("Add Dialogue " + std::to_string(DialgID++));
            tmpIDSwitches.emplace_back(true);
        }
    }*/

    if (ImGui::Button("Add Dialogue", ImVec2(250, 50)))
    {
        ++DialogueID;
        ++DialoguePos;
        DiaName.push_back("##dialgName" + std::to_string(DialogueID));
        DiaContent.push_back("##dialgMain" + std::to_string(DialogueID));
        DiaChoiceA.push_back("##choiceA" + std::to_string(DialogueID));
        DiaChoiceB.push_back("##choiceB" + std::to_string(DialogueID));
        DiaJumpA.push_back("##diaJumpA" + std::to_string(DialogueID));
        DiaJumpB.push_back("##diaJumpB" + std::to_string(DialogueID));
        SusModA.push_back("##susAmtA" + std::to_string(DialogueID));
        SusModB.push_back("##susAmtB" + std::to_string(DialogueID));

        DiaDeleteCursor.push_back("Delete Section " + std::to_string(DialogueID));

        std::cout << "ID : " << DialogueID << "\n";
        std::cout << "Pos : " << DialoguePos << "\n";
    }



    ImGui::End();
}
void SE_RenameObjects()
{
    if (!renameObject_open)
        return;

    ImGui::SetNextWindowSize(ImVec2(500, 200));
    ImGui::SetNextWindowPos(ImVec2(500, 300));
    ImGui::Begin("Rename Objects", &renameObject_open);

    ImGui::Text("Set New Name of Object : ");
    ImGui::SameLine();
    ImGui::Text(current_GO->getName().c_str());
    ImGui::InputText("##newobjname", currGO_newname, 64);


    if (ImGui::Button("Confirm new name"))
    {
        bool newDupeExist = false;
        for (std::pair <const std::string, GameObject*> obj : SEFindObjectsByLevel(currentLevelLoaded))
        {
            if (obj.second->getName() == currGO_newname)
            {
                // NAME EXIST
                newDupeExist = true;
            }
        }

        // New name doesnt exist, = okay to rename 
        if (!newDupeExist)
        {
            current_GO->m_name = currGO_newname;
            renameObject_open = false;
        }
        else
        {
            // NAME EXIST
            NotifWindowMessage = "Name of Object exist! \n Please use another name thanks!";
            NotifWindowSize = ImVec2(500, 200);
            NotifWindowPos = ImVec2(500, 300);
            notifWindow_open = true;
        }


    }

    ImGui::End();
}

void SE_DockSpace()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static bool p_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);
    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}
void SE_Viewport()
{
    static float prev_VPWidth, prev_VPHeight;


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

    ImGui::Begin("ViewPort");

    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    viewportOffset = ImGui::GetWindowPos();
    viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    viewportPanelSize = ImGui::GetContentRegionAvail();





    // check if viewport is modified by user, update prev w and h to current w and h.
    if (prev_VPWidth != viewportPanelSize.x || prev_VPHeight != viewportPanelSize.y)
    {
        prev_VPWidth = viewportPanelSize.x;
        prev_VPHeight = viewportPanelSize.y;

        int ratio = (int)viewportPanelSize.y / 9;    // use height
        new_VPWidth = (float)ratio * 16.0f;           // guess
        new_VPHeight = (float)ratio * 9.0f;

        if (new_VPWidth > viewportPanelSize.x)
        {
            ratio = (int)viewportPanelSize.x / 16;    // use width 
            new_VPWidth = (float)ratio * 16.0f;           // guess
            new_VPHeight = (float)ratio * 9.0f;
        }
        offsetX = (viewportPanelSize.x - new_VPWidth) * 0.5f;
        offsetY = (viewportPanelSize.y - new_VPHeight) * 0.5f;


        resizeable_frameBuffer_Editor((int)new_VPWidth, (int)new_VPHeight);
    }
    auto current_pos = ImGui::GetCursorScreenPos();
    if (offsetX > 0 || offsetY > 0)
    {
        ImGui::SetCursorScreenPos({ current_pos.x + offsetX, current_pos.y + offsetY });
    }
    //auto textures = SEGetFrameBuffer();
    auto textures = SEGetTextureBuffer();
    // Image(framebuffers,...)
    //ImGui::Image((ImTextureID)static_cast<uint64_t>((textures)), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Image((ImTextureID)static_cast<uint64_t>((textures)), ImVec2{ (float)new_VPWidth, (float)new_VPHeight }, ImVec2(0, 1), ImVec2(1, 0));

    SE_Guizmo();
    ImGui::End();
    ImGui::PopStyleVar();

    //ImGui::SetNextWindowPos(ImVec2(0, 0));
    //ImGui::SetNextWindowSize(ImVec2((float)StormGraphics->getWindowWidth(), (float)StormGraphics->getWindowHeight()));
    //// flag : ImGuiWindowFlags_NoResize
    //if (ImGui::Begin("GameWindow", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
    //{
    //    if (ImGui::BeginChild("GameRenderer"))
    //    {
    //        ImVec2 wsize = ImGui::GetWindowSize();
    //        ImGui::Image(reinterpret_cast<void*>((textures)), wsize, ImVec2(0, 1), ImVec2(1, 0));

    //        //Mouse position in the viewport 
    //        auto [mx, my] = ImGui::GetMousePos();
    //        ImVec2 viewportPanelSize;
    //        glm::vec2 viewportBounds[2];
    //        mx -= viewportBounds[0].x;
    //        my -= viewportBounds[0].y;
    //        glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
    //        my = viewportPanelSize.y - my;
    //        int mouseX = (int)mx;
    //        int mouseY = (int)my;

    //        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
    //        {
    //            std::cout << mouseX << " " << mouseY << "\n";
    //        }
    //    }ImGui::EndChild();
    //}
    //ImGui::End();
}

void SE_ContentBrowser()
{
    SELoadTexture("./Assets/Textures/foldericon.png");
    SELoadTexture("./Assets/Textures/fileicon.png");
    const SEUint& m_directoryIcon = SEGetTexture("./Assets/Textures/foldericon.png");
    //const SEUint& m_fileIcon = SEGetTexture("./Assets/Textures/fileicon.png");

    std::filesystem::path m_AssetPath = "./Assets";
    static std::filesystem::path m_currentDirectory(m_AssetPath);


    ImGui::Begin("Content Browser");
    if (m_currentDirectory != std::filesystem::path(m_AssetPath))
    {
        if (ImGui::Button("<-"))
        {
            m_currentDirectory = m_currentDirectory.parent_path();
        }
    }
    static float padding = 16.0f;
    static float thumbnailsize = 128;
    float cellsize = thumbnailsize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columncount = (int)(panelWidth / cellsize);
    if (columncount < 1) columncount = 1;


    ImGui::SliderFloat("Thumbnail Size", &thumbnailsize, 16, 512);
    ImGui::SliderFloat("Padding", &padding, 0, 32);
    ImGui::Columns(columncount, 0, false);

    static bool isLoadedOnHovered = false;
    for (const auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
    {
        const auto& path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path, m_AssetPath);
        std::string filename_str = relativePath.filename().string();

        const SEUint& m_fileIcon = SEGetTexture(relativePath.filename());
        GLuint icon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
        ImGui::ImageButton(reinterpret_cast<ImTextureID*>(icon), { thumbnailsize, thumbnailsize }, { 0,1 }, { 1,0 });
        //ImGui::Button(filename_str.c_str(), { thumbnailsize, thumbnailsize });

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("TexturePayload", path.string().c_str(), path.string().size());
            auto id = SEGetTexture(path);
            ImGui::Image(reinterpret_cast<ImTextureID*>(id), { 128,128 }, { 0,1 }, { 1,0 });
            ImGui::Text("Copy %s", path.string().c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (directoryEntry.is_directory())
                m_currentDirectory /= path.filename();
        }
        ImGui::TextWrapped(filename_str.c_str());

        ImGui::NextColumn();
    }

    ImGui::Columns(1);



    ImGui::End();
}

// gizmo stuff
enum GizmoOperation {
    Translate,
    Rotate,
    Scale
};
GizmoOperation gizmoOperation = Translate;

// ImGuizmo
void SE_Guizmo()
{
    if (current_GO) // game object selected  
    {
        ImGuizmo::SetOrthographic(true); // sets the gizmo to use an orthographic projection
        ImGuizmo::SetDrawlist(); // render gizmo in current frame.

        // Set where gizmo displayed and manipulated
       /* ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y , viewportBounds[1].x
            - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);*/

        ImGuizmo::SetRect(
            viewportBounds[0].x + offsetX, viewportBounds[0].y + offsetY,
            (float)new_VPWidth,
            (float)new_VPHeight);

        //get Camera function
        //glm::mat4 cameraProj = SEGetCameraProj(); // Cam projection
        //glm::mat4 cameraProj = SEGetCameraProj(new_VPWidth, new_VPHeight); // Cam projection
        glm::mat4 cameraProj = glm::mat4
        {
            glm::vec4((float)2.0f / (float)SEGetCameraWidth(), 0, 0, 0),
            glm::vec4(0, (float)2.0f / (float)SEGetCameraHeight(), 0, 0),
            glm::vec4(0, 0, 1, 0),
            glm::vec4(0, 0, 0, 1)
        };

        glm::mat4 cameraView = SEGetCameraMatrix(); // Confirm correct
        cameraView[3] = cameraView[2];
        cameraView[3].w = 1;
        cameraView[2] = { 0,0,1,0 };

        // invert the camera matrix to get the view matrix
        //glm::mat4 viewMatrix = glm::inverse(cameraView);

        //Check if user has pressed a button to change the operation mode
        if (SEIsKeyTriggered(SE_KEY_Z))
            gizmoOperation = Translate;
        if (SEIsKeyTriggered(SE_KEY_X))
            gizmoOperation = Rotate;
        if (SEIsKeyTriggered(SE_KEY_C))
            gizmoOperation = Scale;

        // get the transform component of the selected game object
        Transform* t = (Transform*)current_GO->getComponent(CT_TRANSFORM);

        // matrix components
        if (!t) return;
        glm::mat4 transform = glm::translate(glm::mat4(1.0f),
            glm::vec3(t->getTranslate().x, t->getTranslate().y, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(t->getRotationAngle()), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(t->getScale(), 1.0f));

        bool isScaleXNeg = t->getScale().x < 0.f;
        //Draw ImGuizmo
        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj),
            (gizmoOperation == Translate) ? ImGuizmo::OPERATION::TRANSLATE :
            (gizmoOperation == Rotate) ? ImGuizmo::OPERATION::ROTATE :
            ImGuizmo::OPERATION::SCALE,
            ImGuizmo::LOCAL, glm::value_ptr(transform));

        // decompose the components
        if (ImGuizmo::IsUsing())
        {
            float decompTrans[3], decompRot[3], decompScale[3];

            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), decompTrans, decompRot, decompScale);

            if (isScaleXNeg && decompScale[0] >= 0.f) {
                decompRot[2] -= 180.f;
                decompScale[0] *= -1.f;
            }
            t->setTranslate(SEVec2(decompTrans[0], decompTrans[1]));
            t->setRotationAngle(decompRot[2]);
            t->setScale(SEVec2(decompScale[0], decompScale[1]));
        }
    }
}


void SE_DrawGameObjects()
{
    std::unordered_multimap<std::string, GameObject*> container = SEFindObjectsByLevel(LEVEL::LEVEL_1);

    for (std::pair <const std::string, GameObject*>& obj : container)
    {
        if (obj.second)
        {

            Transform* t = (Transform*)obj.second->getComponent(CT_TRANSFORM);
            Texture* texture = (Texture*)obj.second->getComponent(CT_TEXTURE);

            if (t && texture)
            {

                /*float x = t->getTranslate().x;
                float y = t->getTranslate().y;*/

                //float rot = t->getRotationAngle();
                //float s_x = t->getScale().x;
                //float s_y = t->getScale().y;
                //obj.second->getComponent()


                if (ImGui::Selectable(obj.second->getName().c_str()))
                {
                    current_GO = obj.second;
                    inspector_open = !inspector_open;
                }

                //bool flag = texture->IsAnimating;



               // SEDrawTexture(x, y, s_x, s_y, rot, temp);
            }
        }
    }
}

// Inspector UI Formatting Functions (No Windows in these functions)
void COMP_TransformSection(ImVec2 cursorPos, bool is_inspect)
{
    /************************************************
    *          TODO : GET CURRENT OBJECT
    ************************************************/
    Transform* t = (Transform*)current_GO->getComponent(CT_TRANSFORM);
    static bool hide_transform = false;

    if (t)
    {
        float x = t->getTranslate().x;
        float y = t->getTranslate().y;

        float rot = t->getRotationAngle();
        float s_x = t->getScale().x;
        float s_y = t->getScale().y;

        //float TEMP_X = 0.f;
        //float TEMP_Y = 0.f;



        ImGui::SetCursorPos(cursorPos);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "Transform Component: ");
        ImGui::SameLine();
        if (hide_transform)
        {
            if (ImGui::Button("Show"))
            {
                hide_transform = !hide_transform;
            }
        }
        else
        {
            if (ImGui::Button("Collapse"))
            {
                hide_transform = !hide_transform;
            }
        }
        ImGui::Separator();



        if (!hide_transform)
        {
            /************************************************
            *          Translate Section
            ************************************************/
            ImGui::Text("Translate : ");
            ImGui::DragScalar("Position X", ImGuiDataType_Float, &x, 0.50f, NULL, NULL, "%.2f");
            ImGui::DragScalar("Position Y", ImGuiDataType_Float, &y, 0.50f, NULL, NULL, "%.2f");
            t->setTranslate(glm::vec2(x, y));


            /************************************************
            *          Rotate Section
            ************************************************/
            ImGui::Text("Rotate : ");
            ImGui::DragScalar("(Deg) ", ImGuiDataType_Float, &rot, 0.50f, NULL, NULL, "%.2f");
            t->setRotationAngle(rot);


            /************************************************
            *          Scale Section
            ************************************************/
            ImGui::Text("Scale : ");
            ImGui::DragScalar("Scale X", ImGuiDataType_Float, &s_x, 0.50f, NULL, NULL, "%.2f");
            ImGui::DragScalar("Scale Y", ImGuiDataType_Float, &s_y, 0.50f, NULL, NULL, "%.2f");
            t->setScale(s_x, s_y);


            /************************************************
            *          Delete Button
            ************************************************/
            if (ImGui::Button("Delete Component   "))
            {
                NotifWindowSize = ImVec2(500, 100);
                NotifWindowPos = ImVec2(500, 400);
                NotifWindowMessage = "Cannot delete transform! \n(What, you want something to exist without positions? :P)";
                notifWindow_open = true;
                //inspector_yPos -= transformComponent_height;
                //transformSection_active = false;
                //SEDeleteComponent(current_GO, CT_TRANSFORM);
                //current_GO->deleteComponent(CT_TRANSFORM);
            }



            // Adjust Next Component Display section
            if (is_inspect)
                inspector_yPos += transformComponent_height;
        }
        else
        {
            // Padding to next component header
            if (is_inspect)
                inspector_yPos += 50;
        }
    }
}
void COMP_CollisionSection(ImVec2 cursorPos, bool is_inspect)
{

    /************************************************
    *          GET CURRENT OBJECT
    ************************************************/
    Collision* c = (Collision*)current_GO->getComponent(CT_COLLISION);

    if (c)
    {
        // Get Collision box Position
        //float posX = c->collisionPos.pos.x;
        //float posY = c->collisionPos.pos.y;
        static bool hide_collision = false;

        ImGui::SetCursorPos(cursorPos);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "Collision Component : ");
        ImGui::SameLine();
        if (hide_collision)
        {
            if (ImGui::Button("Show "))
            {
                hide_collision = !hide_collision;
            }
        }
        else
        {
            if (ImGui::Button("Collapse "))
            {
                hide_collision = !hide_collision;
            }
        }
        ImGui::Separator();


        /* float TEMP_X = 0.f;
         float TEMP_Y = 0.f;*/

        if (!hide_collision)
        {
            /************************************************
            *          Collision Rotate Section
            ************************************************/
            //ImGui::Text("Collision Rotate : ");
            //ImGui::DragScalar("(Deg)", ImGuiDataType_Float, &TEMP_X, 0.50f, NULL, NULL, "%.2f");

            /************************************************
            *          Collision Transform Section
            ************************************************/
            ImGui::Text("Collision Position : ");
            static float col_posX = 0; //c->collisionPos.pos.x;
            static float col_posY = 0; //c->collisionPos.pos.y;
            ImGui::DragScalar("Pos X ", ImGuiDataType_Float, &col_posX, 0.50f, NULL, NULL, "%.2f");
            ImGui::DragScalar("Pos Y ", ImGuiDataType_Float, &col_posY, 0.50f, NULL, NULL, "%.2f");

            c->collisionPos.pos.x = c->collisionPos.pos.x + col_posX;
            c->collisionPos.pos.y = c->collisionPos.pos.y + col_posY;



            /************************************************
            *          Collision Scale Section
            ************************************************/
            ImGui::Text("Collision Scale : ");
            float scaleX = c->boundaries.scale.x;
            float scaleY = c->boundaries.scale.y;
            ImGui::DragScalar("Scale X ", ImGuiDataType_Float, &scaleX, 0.50f, NULL, NULL, "%.2f");
            ImGui::DragScalar("Scale Y ", ImGuiDataType_Float, &scaleY, 0.50f, NULL, NULL, "%.2f");


            /************************************************
            *          Collision NEW ASSIGN Section
            ************************************************/
            // Assign new Scale and AABB data
            c->boundaries.scale = StormEngine::Vec2(scaleX, scaleY);
            c->boundaries.min.x = c->collisionPos.pos.x - scaleX;
            c->boundaries.min.y = c->collisionPos.pos.y - scaleY;
            c->boundaries.max.x = c->collisionPos.pos.x + scaleX;
            c->boundaries.max.y = c->collisionPos.pos.y + scaleY;


            /************************************************
            *          Delete Button
            ************************************************/
            if (ImGui::Button("Delete Component  "))
            {
                //c = nullptr;
                //delete c;

                collisionSection_active = false;
                inspector_yPos -= collisionComponent_height;
                SEDeleteComponent(current_GO, CT_COLLISION);
            }

            if (is_inspect)
                inspector_yPos += collisionComponent_height;
        }
        else
        {
            if (is_inspect)
                inspector_yPos += 50;
        }
    }

}
static std::filesystem::path path_texture_preview{};
static std::string previewSprite = "";
void COMP_SpriteSection(ImVec2 cursorPos, bool is_inspect)
{
    Texture* t = (Texture*)current_GO->getComponent(CT_TEXTURE);

    if (t)
    {
        static bool hide_sprite = false;

        ImGui::SetCursorPos(cursorPos);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "Texture Component : ");
        ImGui::SameLine();
        if (hide_sprite)
        {
            if (ImGui::Button("Show  "))
            {
                hide_sprite = !hide_sprite;
            }
        }
        else
        {
            if (ImGui::Button("Collapse  "))
            {
                hide_sprite = !hide_sprite;
            }
        }
        ImGui::Separator();

        if (!hide_sprite)
        {
            std::string obj_Layer = t->GetCurrentRenderLayer();

            ImGui::Text("Current Render Layer : ");
            ImGui::SameLine();
            ImGui::Text(obj_Layer.c_str());

            ImGui::Text("Set Render Priority : ");
            ImGui::SameLine();
            ImGui::PushItemWidth(100.f);

            // RENDER PRIORITY ASSIGN HERE
            ImGui::InputInt("##rendOrd   ", &t->m_priority_level);

            SELayer* foundlayer = SELayerStackManager->SEFindLayer(t->GetCurrentRenderLayer());

            // Every time priority is assigned, order need to be sorted
            if (foundlayer)
            {
                SELayerStackManager->SEFindLayer(t->GetCurrentRenderLayer())->SortObjectPriority();
            }

            ImGui::PopItemWidth();

            Texture* pTexture = (Texture*)current_GO->getComponent(CT_TEXTURE);
            if (pTexture)
            {
                for (const auto& it : pTexture->AnimationTextures[ACTION::ACTION_NULL])
                {
                    path_texture_preview = it.second;
                }
            }

            /////////////////////////////////////////////////////////////
            // This section is for changing the current sprite's TEXTURE
            COMP_TextureSection();



            //ImGui::Checkbox("Is Animating", &is_SpriteSheet);
            ImGui::Checkbox("Keep Aspect Ratio", &t->KeepAspectRatio);
            COMP_AnimationSection();


            /////////////////////////////////////////////////////////////
            // This section is for changing the current sprite's ANIMATION
            /*if (is_SpriteSheet)
            {
                std::cout << "Animating\n";
                COMP_AnimationSection();
            }
            else
            {
                std::cout << "Not Animating\n";
                Texture* pTexture = (Texture*)current_GO->getComponent(CT_TEXTURE);
                if (pTexture)
                {
                    pTexture->isRendering = 1;
                    if (path_texture_preview.empty()) return;
                    SELoadTexture(path_texture_preview);
                    pTexture->AnimationTextures[ACTION::ACTION_NULL].clear();
                    pTexture->AnimationTextures[ACTION::ACTION_NULL].insert({ path_texture_preview.string(), path_texture_preview });
                    pTexture->Rows[ACTION::ACTION_NULL] = 1;
                    pTexture->Columns[ACTION::ACTION_NULL] = 1;
                    pTexture->AnimationFrames[ACTION::ACTION_NULL] = 1;
                    pTexture->AnimationSpeed[ACTION::ACTION_NULL] = 1;
                    pTexture->AnimationUV[ACTION::ACTION_NULL] = pTexture->SEComputeUV(1, 1, 1);
                }
            }*/


            // TODO
            /************************************************
            *          Delete Button
            ************************************************/
            if (ImGui::Button("Delete Component    "))
            {
                //inspector_yPos -= textureComponent_height;
                //textureSection_active = false;
                //SEDeleteComponent(current_GO, CT_TEXTURE);
            }


            // End of section
            if (is_inspect)
                inspector_yPos += textureComponent_height;
        }
        else
        {
            // Creates offset when section is hidden
            if (is_inspect)
                inspector_yPos += 50;
        }
    }

}

char buffer[1024];
void COMP_TextureSection()//ImVec2 cursorPos, bool is_inspect)
{

    ImGui::Text("Current Sprite : ");

    Texture* pTexture = (Texture*)current_GO->getComponent(CT_TEXTURE);
    if (pTexture)
    {
        //// Initialization ===================================================
        //for (int i = 0; i < spriteList.size(); i++)
        //{
        //    // Initialized the preview string to the current sprite (If it exists)
        //    const auto& map = pTexture->AnimationTextures[ACTION::ACTION_NULL];
        //    for (const auto& tmp : map)
        //    {
        //        path_texture_preview = tmp.second;
        //    }

        //    //if (pTexture->m_texture_path == spriteList[i].c_str())
        //    if (strcmp(path_texture_preview.string().c_str(), spriteList[i].c_str()) == 0)
        //    {
        //        previewSprite = spriteList[i].c_str();
        //        spriteCursor[i] = true;
        //        // Deselect any non selected options
        //        for (int j = 0; j < spriteList.size(); j++)
        //        {
        //            if (j != i)
        //                spriteCursor[j] = false;
        //        }
        //        break;
        //    }
        //    else
        //    {
        //        // Has texture component but no sprite selected
        //        previewSprite = "";
        //    }

        //}
        ImGui::InputText("placeholder", buffer, 1024);
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TexturePayload"))
            {
                strcpy_s(buffer, 1024, static_cast<const char*>(payload->Data));
                buffer[payload->DataSize] = '\0';
                const std::filesystem::path tmp = buffer;
                path_texture_preview = buffer;
                if (!tmp.empty())
                {
                    std::cout << "loading " << path_texture_preview << std::endl;
                    SELoadTexture(path_texture_preview);
                    pTexture->AnimationUV[ACTION::ACTION_NULL] = pTexture->SEComputeUV(1, 1, 1);
                    pTexture->AnimationTextures[ACTION::ACTION_NULL].clear();
                    pTexture->AnimationTextures[ACTION::ACTION_NULL].insert({ path_texture_preview.string(), path_texture_preview });
                }
            }
            ImGui::EndDragDropTarget();
        }
        else
        {
            std::string texture_str{};
            for (const auto& texture : pTexture->AnimationTextures[ACTION::ACTION_NULL])
            {
                texture_str = texture.first;
            }
            texture_str.copy(buffer, 1024);
        }
        // Drop Down ===================================================
        //if (ImGui::BeginCombo("  ", previewSprite.c_str()))
        //{
        //    // Drop down for Sprite Selection
        //    for (int i = 0; i < spriteList.size(); i++)
        //    {
        //        // SpriteCursor is a list of bools to tell us which sprite is being selected
        //        ImGui::Selectable(spriteList[i].c_str(), &spriteCursor[i]);

        //        if (spriteCursor[i])
        //        {
        //            // Display selected string
        //            path_texture_preview = previewSprite = spriteList[i].c_str();

        //           // pTexture->m_texture_path = previewSprite;


        //            std::filesystem::path path = previewSprite;
        //            SELoadTexture(path);
        //            std::string name = path.filename().string();
        //            pTexture->AnimationTextures.clear();
        //            pTexture->AnimationTextures[ACTION::ACTION_NULL].insert(std::make_pair(name, path));
        //        }
        //        // Deselect any non selected options
        //        for (int j = 0; j < spriteList.size(); j++)
        //        {
        //            if (j != i)
        //                spriteCursor[j] = false;
        //        }
        //    }
        //    ImGui::EndCombo();
        //}


    }

}

static int anim_state = 0;     // Animation State ID : (see "std::vector<const char*>  AnimationState" at the top of this cpp)
std::string FixStateDisplay = AnimationState[anim_state];
void COMP_AnimationSection()
{
    static ACTION currAction = ACTION::ACTION_NULL;
    Texture* pTexture = (Texture*)current_GO->getComponent(CT_TEXTURE);
    /*static int animRows = 1;
    static int animCols = 1;
    static int maxFrames = 1;
    static int animSpeed = 1;*/

    ImGui::Separator();
    ImGui::TextColored(ImVec4(0, .7f, 1, 1), "Animation Settings");

    // Temporary way to change/display different animation states
   /* ImGui::Text("Change Animation State");
    ImGui::SameLine();
    if (ImGui::Button("-", ImVec2(20, 20))) { (anim_state <= 0) ? anim_state = 0 : --anim_state; };
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(20, 20))) { (anim_state >= 3) ? anim_state = 3 : ++anim_state; };*/

    //std::cout << "my rows : " << animRows << "\n";

    // (DEPRECATED : Tech lead say no need, upgrade to drop down)
    // Display current animation state.     
    //  Unchangable at the moment as its only meant to show current state
    //ImGui::InputText("##DisplayAnimState", (char*)noStateInputLOL.c_str(), 64);

    // Drop down for Animation State Selection
    //if (ImGui::BeginCombo("##DisplayAnimState", FixStateDisplay.c_str()))
    //{
    //    for (int i = 0; i < AnimationState.size(); i++)
    //    {
    //        if (ImGui::Selectable(((std::string)AnimationState[i]).c_str(), &AnimStateCursor[i]))
    //        {
    //            //if (AnimationState[i] == "Idle")
    //            //{
    //            //    currAction = ACTION::ACTION_IDLE;
    //            //}
    //            //if (AnimationState[i] == "Cleaning")
    //            //{
    //            //    currAction = ACTION::ACTION_CLEANING;
    //            //}
    //            //if (AnimationState[i] == "Walking")
    //            //{
    //            //    currAction = ACTION::ACTION_WALKING;
    //            //}

    //            //animRows    = pTexture->Rows[currAction];
    //            //animCols    = pTexture->Columns[currAction];
    //            //animSpeed   = (int)pTexture->AnimationSpeed[currAction];
    //            //maxFrames   = pTexture->AnimationFrames[currAction];

    //            //if (AnimStateCursor[i])
    //            //{
    //            //    // Display selected string
    //            //    FixStateDisplay = AnimationState[i];

    //            //    // Deselect any non selected options
    //            //    for (int j = 0; j < AnimationState.size(); j++)
    //            //    {
    //            //        if (j != i)
    //            //            AnimStateCursor[j] = false;
    //            //    }
    //            //}
    //        }
    //    }

    //    ImGui::EndCombo();
    //}



    // Display sprite sheet rows and columns

    ImGui::PushItemWidth(100.f);
    ImGui::Text("Rows    : ");
    ImGui::SameLine();
    ImGui::InputInt("##sprSheetRows", &pTexture->Rows[ACTION::ACTION_NULL], 0);    // todo : assign data
    ImGui::Text("Columns : ");
    ImGui::SameLine();
    ImGui::InputInt("##sprSheetCols", &pTexture->Columns[ACTION::ACTION_NULL], 0);    // todo : assign data
    ImGui::PopItemWidth();



    // Display Frame Data
    //int maxFrames = 99;         
    ImGui::Text("TOTAL FRAMES : ", 99); ImGui::SameLine();
    ImGui::PushItemWidth(100.f);
    ImGui::InputInt("##sprTotalFrame", &pTexture->AnimationFrames[ACTION::ACTION_NULL]);    // TODO : maxframe IS FOUND AT TOP OF "THIS" FUNCTION (not at the top of cpp!)
    ImGui::PopItemWidth();



    // Animation Speed Settings
    ImGui::Text("Animation Speed : "); ImGui::SameLine();
    ImGui::PushItemWidth(100.f);
    ImGui::InputFloat("##sprAnimSpd", &pTexture->AnimationSpeed[ACTION::ACTION_NULL]);    // TODO : replace temp int data with anim speed 
    ImGui::PopItemWidth();
    pTexture->isRendering = 1;

    SELoadTexture(path_texture_preview);
    pTexture->AnimationUV[ACTION::ACTION_NULL] = pTexture->SEComputeUV(pTexture->Rows[ACTION::ACTION_NULL], pTexture->Columns[ACTION::ACTION_NULL], pTexture->AnimationFrames[ACTION::ACTION_NULL]);
    pTexture->AnimationTextures[ACTION::ACTION_NULL].clear();
    pTexture->AnimationTextures[ACTION::ACTION_NULL].insert({ path_texture_preview.string(), path_texture_preview });
    /*if (!path_texture_preview.empty())
    {
        SELoadTexture(path_texture_preview);
        pTexture->AnimationUV[ACTION::ACTION_NULL] = pTexture->SEComputeUV(pTexture->Rows[ACTION::ACTION_NULL], pTexture->Columns[ACTION::ACTION_NULL], pTexture->AnimationFrames[ACTION::ACTION_NULL]);
        pTexture->AnimationTextures[ACTION::ACTION_NULL].clear();
        pTexture->AnimationTextures[ACTION::ACTION_NULL].insert({ path_texture_preview.string(), path_texture_preview });
    }*/
    /* if(path_texture_preview.empty())
     {
         std::filesystem::path tmp_missing_texture = "./Assets/Textures/missing.png";
         SELoadTexture(tmp_missing_texture);
         pTexture->AnimationUV[ACTION::ACTION_NULL] = pTexture->SEComputeUV(pTexture->Rows[ACTION::ACTION_NULL], pTexture->Columns[ACTION::ACTION_NULL], pTexture->AnimationFrames[ACTION::ACTION_NULL]);
         pTexture->AnimationTextures[ACTION::ACTION_NULL].clear();
         pTexture->AnimationTextures[ACTION::ACTION_NULL].insert({ tmp_missing_texture.string(), tmp_missing_texture });
     }*/
     //pTexture->Rows[ACTION::ACTION_NULL] = animRows;
     //pTexture->Columns[ACTION::ACTION_NULL] = animCols;
     //pTexture->AnimationFrames[ACTION::ACTION_NULL] = maxFrames;
     //pTexture->AnimationSpeed[ACTION::ACTION_NULL] = (float)animSpeed;

     // old Assigning code
     /*for (int i = 0; i < AnimationState.size(); i++)
     {
         if (AnimStateCursor[i])
         {
             if (AnimationState[i] == "Idle")
             {
                 pTexture->IsAnimating = 1;
                 pTexture->AnimationTextures[ACTION::ACTION_IDLE].emplace(std::make_pair(pTexture->m_texture_path.filename().string(), pTexture->m_texture_path));
                 pTexture->Rows[ACTION::ACTION_IDLE] = animRows;
                 pTexture->Columns[ACTION::ACTION_IDLE] = animCols;
                 pTexture->AnimationSpeed[ACTION::ACTION_IDLE] = animSpeed;
                 pTexture->AnimationFrames[ACTION::ACTION_IDLE] = maxFrames;
                 auto tmp_uv = pTexture->SEComputeUV(pTexture->Rows[ACTION::ACTION_IDLE], pTexture->Columns[ACTION::ACTION_IDLE], pTexture->AnimationFrames[ACTION::ACTION_IDLE]);
                 pTexture->AnimationUV[ACTION::ACTION_IDLE] = tmp_uv;
             }

             if (AnimationState[i] == "Cleaning")
             {
                 pTexture->IsAnimating = 1;
                 pTexture->AnimationTextures[ACTION::ACTION_CLEANING].emplace(std::make_pair(pTexture->m_texture_path.filename().string(), pTexture->m_texture_path));
                 pTexture->Rows[ACTION::ACTION_CLEANING] = animRows;
                 pTexture->Columns[ACTION::ACTION_CLEANING] = animCols;
                 pTexture->AnimationSpeed[ACTION::ACTION_CLEANING] = animSpeed;
                 pTexture->AnimationFrames[ACTION::ACTION_CLEANING] = maxFrames;
                 auto tmp_uv = pTexture->SEComputeUV(pTexture->Rows[ACTION::ACTION_CLEANING], pTexture->Columns[ACTION::ACTION_CLEANING], pTexture->AnimationFrames[ACTION::ACTION_CLEANING]);
                 pTexture->AnimationUV[ACTION::ACTION_CLEANING] = tmp_uv;
             }

         }
     }*/

    inspector_yPos += 150.f;


}

void COMP_ButtonGUISection(ImVec2 cursorPos, bool is_inspect)
{
    // TODO : Get Button Component
    //ButtonGUI* t = (ButtonGUI*)current_GO->getComponent(CT_BUTTONGUI);

    static bool hide_btnGUI = false;

    ImGui::SetCursorPos(cursorPos);
    ImGui::Separator();
    ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "ButtonGUI Component : ");
    ImGui::SameLine();
    if (hide_btnGUI)
    {
        if (ImGui::Button("Show   "))
        {
            hide_btnGUI = !hide_btnGUI;
        }
    }
    else
    {
        if (ImGui::Button("Collapse   "))
        {
            hide_btnGUI = !hide_btnGUI;
        }
    }
    ImGui::Separator();

    if (!hide_btnGUI)
    {
        float tmpHovSz_x = 0.f;
        float tmpHovSz_y = 0.f;

        float tmpClkSz_x = 0.f;
        float tmpClkSz_y = 0.f;

        /* float tmphovLT = 0.f;
         float tmpclkLT = 0.f;*/

#pragma region HOVER BUTTONGUI SETTINGS
        ImGui::TextColored(ImVec4(0, .7f, 1, 1), "On Hover"); ImGui::SameLine();
        static bool subHoverHide = false;
        if (ImGui::Button("Hide Details"))
            subHoverHide = !subHoverHide;
        if (!subHoverHide)
        {
            ImGui::Text("Hover Texture :");
            static std::string previewHover = "";
            // TODO 
            /*
             - Get Current Sprite here
             - set "previewSprite" to name of Current sprite
            */
            // Drop down for Hover Sprite Selection
            if (ImGui::BeginCombo("##hoverSprList", previewHover.c_str()))
            {
                previewHover = "";

                for (int i = 0; i < spriteList.size(); i++)
                {
                    // SpriteCursor is a list of bools to tell us which sprite is being selected
                    ImGui::Selectable(spriteList[i].c_str(), &spriteCursor[i]);

                    if (spriteCursor[i])
                    {
                        // Display selected string
                        previewHover = spriteList[i].c_str();

                        // Deselect any non selected options
                        for (int j = 0; j < spriteList.size(); j++)
                        {
                            if (j != i)
                                spriteCursor[j] = false;
                        }
                    }
                }

                ImGui::EndCombo();
            }
            ImGui::Text("Size on Hover :"); ImGui::SameLine();
            ImGui::PushItemWidth(50.f); ImGui::InputFloat("##hoverSzX", &tmpHovSz_x, 0); ImGui::SameLine();
            ImGui::InputFloat("##hoverSzY", &tmpHovSz_y, 0); ImGui::PopItemWidth();
            ImGui::Checkbox("Enable Animation", &animHover);
            if (animHover)
            {
                // Display sprite sheet rows and columns
                int animRows2 = 0;   // temp data (TODO)
                int animCols2 = 0;   // temp data (TODO)
                ImGui::PushItemWidth(100.f);
                ImGui::Text("Rows   : ");
                ImGui::SameLine();
                ImGui::InputInt("##sprSheetRows ", &animRows2, 0);    // todo : assign data
                ImGui::Text("Columns :");
                ImGui::SameLine();
                ImGui::InputInt("##sprSheetCols ", &animCols2, 0);    // todo : assign data
                ImGui::PopItemWidth();

                int maxFrames2 = 0;

                // Display Frame Data
                //int maxFrames = 99;         
                ImGui::Text("TOTAL FRAMES :", maxFrames2); ImGui::SameLine();
                ImGui::PushItemWidth(100.f);
                ImGui::InputInt("##sprTotalFrame ", &maxFrames2);    // TODO : maxframe IS FOUND AT TOP OF "THIS" FUNCTION (not at the top of cpp!)
                ImGui::PopItemWidth();



                // Animation Speed Settings
                int animSpeed = 0;
                ImGui::Text("Animation Speed :"); ImGui::SameLine();
                ImGui::PushItemWidth(100.f);
                ImGui::InputInt("##sprAnimSpd ", &animSpeed);    // TODO : replace temp int data with anim speed 
                ImGui::PopItemWidth();

                inspector_yPos += 100.f;
            }
        }
        else
            inspector_yPos -= 80.f;

#pragma endregion HOVER BUTTONGUI SETTINGS


        ImGui::Separator();


#pragma region ONCLICK BUTTONGUI SETTINGS
        ImGui::TextColored(ImVec4(0, .7f, 1, 1), "On Click"); ImGui::SameLine();
        static bool subClickHide = false;
        if (ImGui::Button("Hide Details "))
            subClickHide = !subClickHide;
        if (!subClickHide)
        {
            ImGui::Text("Click Texture :");
            static std::string previewClick = "";
            // TODO 
            /*
             - Get Current Sprite here
             - set "previewSprite" to name of Current sprite
            */
            // Drop down for Hover Sprite Selection
            if (ImGui::BeginCombo("##clickSprList", previewClick.c_str()))
            {
                previewClick = "";

                for (int i = 0; i < spriteList.size(); i++)
                {
                    // SpriteCursor is a list of bools to tell us which sprite is being selected
                    ImGui::Selectable(spriteList[i].c_str(), &spriteCursor[i]);

                    if (spriteCursor[i])
                    {
                        // Display selected string
                        previewClick = spriteList[i].c_str();

                        // Deselect any non selected options
                        for (int j = 0; j < spriteList.size(); j++)
                        {
                            if (j != i)
                                spriteCursor[j] = false;
                        }
                    }
                }

                ImGui::EndCombo();
            }
            ImGui::Text("Size on Click :"); ImGui::SameLine();
            ImGui::PushItemWidth(50.f); ImGui::InputFloat("##clickSzX", &tmpClkSz_x, 0); ImGui::SameLine();
            ImGui::InputFloat("##clickSzY", &tmpClkSz_y, 0); ImGui::PopItemWidth();
            ImGui::Checkbox("Enable Animation ", &animClick);
            if (animClick)
            {
                // Display sprite sheet rows and columns
                int animRows3 = 0;   // temp data (TODO)
                int animCols3 = 0;   // temp data (TODO)
                ImGui::PushItemWidth(100.f);
                ImGui::Text("Rows  :");
                ImGui::SameLine();
                ImGui::InputInt("##sprSheetRows  ", &animRows3, 0);    // todo : assign data
                ImGui::Text("Columns:");
                ImGui::SameLine();
                ImGui::InputInt("##sprSheetCols  ", &animCols3, 0);    // todo : assign data
                ImGui::PopItemWidth();


                int maxFrames3 = 0;
                // Display Frame Data
                //int maxFrames = 99;         
                ImGui::Text("TOTAL FRAMES:", maxFrames3); ImGui::SameLine();
                ImGui::PushItemWidth(100.f);
                ImGui::InputInt("##sprTotalFrame  ", &maxFrames3);    // TODO : maxframe IS FOUND AT TOP OF "THIS" FUNCTION (not at the top of cpp!)
                ImGui::PopItemWidth();



                // Animation Speed Settings
                int animSpeed = 0;
                ImGui::Text("Animation Speed  :"); ImGui::SameLine();
                ImGui::PushItemWidth(100.f);
                ImGui::InputInt("##sprAnimSpd  ", &animSpeed);    // TODO : replace temp int data with anim speed 
                ImGui::PopItemWidth();

                inspector_yPos += 100.f;
            }
        }
        else
            inspector_yPos -= 80.f;
#pragma endregion ONCLICK BUTTONGUI SETTINGS


        ImGui::Separator();


        ImGui::TextColored(ImVec4(0, .7f, 1, 1), "Button Function");
        static std::string previewGuiFunc = "";
        if (ImGui::BeginCombo("##GUIFunclist ", previewGuiFunc.c_str()))
        {

            for (int i = 0; i < GUIFuncList.size(); i++)
            {
                ImGui::Selectable(GUIFuncList[i], &GUIFuncCursor[i]);
                if (GUIFuncCursor[i])
                {
                    // Display selected string
                    previewGuiFunc = GUIFuncList[i];
                    GUIFuncCursor[i] = true;

                    // Deselect any non selected options
                    /*for (int j = 0; j < GUIFuncList.size(); j++)
                    {
                        if (j != i)
                            GUIFuncCursor[j] = false;
                    }*/

                }
            }

            ImGui::EndCombo();

        }

        for (int i = 0; i < GUIFuncList.size(); i++)
        {
            /*
            "Change Scene",
            "Toggle UI",
            "Change Dialogue",
            "Add Suspicion",
            "Reduce Suspicion",
            "Pause Game",
            "Quit Game"
            */
            if (GUIFuncCursor[i])
            {
                switch (i)
                {
                case 0:     // change scene
                    COMP_BTNSwitchScenes();
                    inspector_yPos += 50.f;
                    break;
                case 1:     // toggle ui
                    COMP_BTNToggleUI();
                    inspector_yPos += 80.f;
                    break;
                case 2:     // Change dialogue
                    break;
                case 3:     // Add Suspicion
                    break;
                default:
                    break;
                }
            }

        }


        if (ImGui::Button("Delete Component   "))
        {
            //c = nullptr;
            //delete c;

            //DialogueSection_active = false;
            //inspector_yPos -= dialogue_height;
            //SEDeleteComponent(current_GO, CT_DIALOGUE);
        }

        // End of section
        if (is_inspect)
            inspector_yPos += buttonGUI_height;
    }
    else
    {
        if (is_inspect)
            inspector_yPos += 50;
    }
}
void COMP_BTNSwitchScenes()
{
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.5f, 1.0f), "Change Scene to :");
    if (ImGui::BeginCombo("##levellist ", previewLevelName.c_str()))
    {
        for (int i = 0; i < LevelList.size(); i++)
        {
            // Populate the list with selectable levels
            ImGui::Selectable(LevelList[i], &levelCursor[i]);
            if (levelCursor[i])
            {
                // Display selected string
                previewLevelName = LevelList[i];
                levelCursor[i] = true;

                // Deselect any non selected options
                for (int j = 0; j < LevelList.size(); j++)
                {
                    if (j != i)
                        levelCursor[j] = false;
                }
            }
        }

        ImGui::EndCombo();
    }
}
void COMP_BTNToggleUI()
{
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.5f, 1.0f), "Set UI state on click");

    static std::string previewTargetGUI = "";
    ImGui::Text("Target UI :");
    if (ImGui::BeginCombo("##existUIlist ", previewTargetGUI.c_str()))
    {
        // Go thru all layers
        for (auto layer_go : SELayerStackManager->m_layer_stack)
        {
            // Check if the layer in loop is the layer we selected
            // "rendLayerString" is set in SE_UIDrawLayerRender() everytime the render layer button is pressed
            if (layer_go->GetLayerName() == "UI Elements")
            {
                // if layer is the one we want, list all the objs in that layer
                for (auto go : layer_go->GetLayerObjects())
                {
                    Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);

                    if (pTexture)
                    {
                        if (ImGui::Selectable(go->getName().c_str()))
                        {
                            previewTargetGUI = go->getName();

                        }
                    }
                }
            }

        }
        //for (int i = 0; i < GUI_UIList.size(); i++)
        //{
        //    // TODO : GET EXISTING UI FROM OVERLAY / UI ELEMENT LAYERS
        //    // Populate the list with selectable UI
        //    ImGui::Selectable(GUI_UIList[i], &GUIUICursor[i]);
        //    if (GUIUICursor[i])
        //    {
        //        // Display selected string
        //        previewTargetGUI = GUI_UIList[i];
        //        GUIUICursor[i] = true;
        //        // Deselect any non selected options
        //        for (int j = 0; j < GUI_UIList.size(); j++)
        //        {
        //            if (j != i)
        //                GUIUICursor[j] = false;
        //        }
        //    }
        //}

        ImGui::EndCombo();
    }

    static bool tempToHideUI = false;
    ImGui::Text("Action on click :"); ImGui::SameLine();
    ImGui::Checkbox("##hideUI", &tempToHideUI);
    if (tempToHideUI)
        ImGui::Text("[UI will SHOW on click.]");
    else
        ImGui::Text("[UI will HIDE on click.]");

}
void COMP_DialogueSection(ImVec2 cursorPos, bool is_inspect)
{
    Dialogue* d = (Dialogue*)current_GO->getComponent(CT_DIALOGUE);

    static bool hide_dialg = false;

    if (d)
    {
        ImGui::SetCursorPos(cursorPos);
        ImGui::Separator();
        ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "Dialogue Component: ");
        ImGui::SameLine();
        if (hide_dialg)
        {
            if (ImGui::Button("Show    "))
            {
                hide_dialg = !hide_dialg;
            }
        }
        else
        {
            if (ImGui::Button("Collapse    "))
            {
                hide_dialg = !hide_dialg;
            }
        }
        ImGui::Separator();

        if (!hide_dialg)
        {
            if (ImGui::Button("OPEN DIALOGUE INTERFACE", ImVec2(250, 50)))
            {
                dialogueManager_open = true;
            }

            if (ImGui::Button("Delete Component    "))
            {
                //c = nullptr;
                //delete c;

                //DialogueSection_active = false;
                //inspector_yPos -= dialogue_height;
                //SEDeleteComponent(current_GO, CT_DIALOGUE);
            }

            // Adjust Next Component Display section
            if (is_inspect)
                inspector_yPos += dialogue_height;
        }
        else
        {
            // Creates offset when section is hidden
            if (is_inspect)
                inspector_yPos += 50;
        }
    }
}
void COMP_TextSection(ImVec2 cursorPos, bool is_inspect)
{
    static bool hide_textComp = false;

    //if (d)
    //{
    ImGui::SetCursorPos(cursorPos);
    ImGui::Separator();
    ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "Text Component: ");
    ImGui::SameLine();
    if (hide_textComp)
    {
        if (ImGui::Button("Show     "))
        {
            hide_textComp = !hide_textComp;
        }
    }
    else
    {
        if (ImGui::Button("Collapse     "))
        {
            hide_textComp = !hide_textComp;
        }
    }
    ImGui::Separator();

    if (!hide_textComp)
    {
        static char temp_text[64] = "";
        static float temp = 0.f;
        static bool tempSwitch = false;

        ImGui::Text("Text : ");
        //ImGui::SameLine();
        ImGui::InputTextMultiline("##textbody", temp_text, IM_ARRAYSIZE(temp_text),        // "dialgMain" CONTAINS THE ACTUAL DIALOGUE
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5), ImGuiInputTextFlags_AllowTabInput);   // Last 2 parameters just specifies size of input box
        ImGui::Text("Font Size : ");
        ImGui::SameLine();
        ImGui::PushItemWidth(50.f);
        ImGui::DragScalar("##fontSize", ImGuiDataType_Float, &temp, 0.50f, NULL, NULL, "%.f");
        ImGui::PopItemWidth();
        ImGui::Checkbox("Is clickable", &tempSwitch);
        /*ImGui::SameLine();
        ImGui::DragScalar("##wpY", ImGuiDataType_Float, &temp_wpY, 0.50f, NULL, NULL, "%.1f");
        ImGui::PopItemWidth();

        ImGui::Text("Waypoint 2 : ");
        ImGui::SameLine();
        ImGui::PushItemWidth(50.f);
        ImGui::DragScalar("##wpX", ImGuiDataType_Float, &temp_wpX, 0.50f, NULL, NULL, "%.1f");
        ImGui::SameLine();
        ImGui::DragScalar("##wpY", ImGuiDataType_Float, &temp_wpY, 0.50f, NULL, NULL, "%.1f");
        ImGui::PopItemWidth();*/


        if (ImGui::Button("Delete Component     "))
        {
            //c = nullptr;
            //delete c;

            //DialogueSection_active = false;
            //inspector_yPos -= dialogue_height;
            //SEDeleteComponent(current_GO, CT_DIALOGUE);
        }

        // Adjust Next Component Display section
        if (is_inspect)
            inspector_yPos += textComp_height;
    }
    else
    {
        // Creates offset when section is hidden
        if (is_inspect)
            inspector_yPos += 50;
    }

    //}
}
void COMP_WaypointsSection(ImVec2 cursorPos, bool is_inspect)
{
    static bool hide_waypnts = false;

    //if (d)
    //{
    ImGui::SetCursorPos(cursorPos);
    ImGui::Separator();
    ImGui::TextColored(ImVec4(.7f, 1.0f, 0.0f, 1.0f), "Waypoints Component: ");
    ImGui::SameLine();
    if (hide_waypnts)
    {
        if (ImGui::Button("Show      "))
        {
            hide_waypnts = !hide_waypnts;
        }
    }
    else
    {
        if (ImGui::Button("Collapse      "))
        {
            hide_waypnts = !hide_waypnts;
        }
    }
    ImGui::Separator();

    if (!hide_waypnts)
    {
        float temp_wpX = 0.f;
        float temp_wpY = 0.f;

        ImGui::Text("Waypoint 1 : ");
        ImGui::SameLine();
        ImGui::PushItemWidth(50.f);
        ImGui::DragScalar("##wpX", ImGuiDataType_Float, &temp_wpX, 0.50f, NULL, NULL, "%.1f");
        ImGui::SameLine();
        ImGui::DragScalar("##wpY", ImGuiDataType_Float, &temp_wpY, 0.50f, NULL, NULL, "%.1f");
        ImGui::PopItemWidth();

        ImGui::Text("Waypoint 2 : ");
        ImGui::SameLine();
        ImGui::PushItemWidth(50.f);
        ImGui::DragScalar("##wpX", ImGuiDataType_Float, &temp_wpX, 0.50f, NULL, NULL, "%.1f");
        ImGui::SameLine();
        ImGui::DragScalar("##wpY", ImGuiDataType_Float, &temp_wpY, 0.50f, NULL, NULL, "%.1f");
        ImGui::PopItemWidth();


        if (ImGui::Button("Delete Component      "))
        {
            //c = nullptr;
            //delete c;

            //DialogueSection_active = false;
            //inspector_yPos -= dialogue_height;
            //SEDeleteComponent(current_GO, CT_DIALOGUE);
        }

        // Adjust Next Component Display section
        if (is_inspect)
            inspector_yPos += waypoints_height;
    }
    else
    {
        // Creates offset when section is hidden
        if (is_inspect)
            inspector_yPos += 50;
    }

    //}
}



glm::vec3 SEGetImguiMouse()
{
    // For Ref... (Not used)
    //glm::vec3 mouse = { SEGetImguiMouseX() * 2 / (float)SEGetWindowWidth() - 1.f, (SEGetImguiMouseY() * 2 / (float)SEGetWindowHeight()) - 1.0f , 1 };
    //auto mat = SEGetWorldToNDCMatrix();
    //mat = glm::inverse(mat);
    //auto newMousePos = mat * mouse;

    // MVP: model -> world -> view -> projection -> ndc -> framebuffer

    //Convert mouse position in imgui viewport relative to opengl viewport
    auto mouse_x = ((ImGui::GetMousePos().x - viewportOffset.x - offsetX) / new_VPWidth * (float)SEGetWindowWidth());
    auto mouse_y = ((ImGui::GetMousePos().y - viewportOffset.y - offsetY - 20.0f) / new_VPHeight * (float)SEGetWindowHeight());

    // convert mouse position in opengl viewport to NDC range
    auto mouse_x_ndc = mouse_x * 2 / SEGetWindowWidth() - 1.0f;
    auto mouse_y_ndc = 1.0f - mouse_y * 2 / SEGetWindowHeight();
    glm::vec3 mouse{ mouse_x_ndc, mouse_y_ndc, 1 };

    // convert ndc system to world system
    auto ndc_to_world_mat = glm::inverse(SEGetWorldToNDCMatrix());

    // get mouse position in world system
    return ndc_to_world_mat * mouse;
}

std::string SESaveFile(const char* filter, GLFWwindow* window)
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window(window);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.lpstrTitle = "Save File";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetSaveFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }
    return std::string();
}

std::string SEOpenFile(const char* filter, GLFWwindow* window)
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window(window);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.lpstrTitle = "Open File";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }
    return std::string();
}

//
//float SEGetImguiMouseX()
//{
//    // MVP: model -> world -> view -> projection -> ndc -> framebuffer
//
//    // Convert mouse position in imgui viewport relative to opengl viewport
//    auto x_world_system = ((SEGetCursorX() - viewportOffset.x )/ viewportPanelSize.x * (float)SEGetWindowWidth());
//    
//    // convert mouse position in opengl viewport to NDC range
//    auto x_ndc_system = x_world_system * 2 / SEGetWindowWidth() - 1.0f;
//    
//    // convert ndc system to world system
//    auto ndc_to_world_mat = glm::inverse(SEGetWorldToNDCMatrix());
//
//    // get mouse position in world system
//    auto mouse_world_system = ndc_to_world_mat * x_world_system;
//    return mouse_world_system;
//}
//float SEGetImguiMouseY()
//{
//    return ((SEGetCursorY() - viewportOffset.y) / viewportPanelSize.y * (float)SEGetWindowHeight());
//}

void SE_LoadingWindow(std::string task) {

}