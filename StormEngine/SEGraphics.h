/******************************************************************************//**
 * \file		SEGraphics.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (100%)
 *
 * \brief		Storm Engine Graphics Library
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once
#include "pch.h"
#include "SEResource.h"

 /********************************************************************************************
  * Key presses for keyboard and mouse														 *
  * Note: To check for mouse input, use SEIsMouseRightPressed and SEIsMouseLeftPressed.		 *
  * Don't use KEY_F12!!!																	 *
 *********************************************************************************************/

#define SE_KEY_SPACE              32
#define SE_KEY_APOSTROPHE         39  /* ' */
#define SE_KEY_COMMA              44  /* , */
#define SE_KEY_MINUS              45  /* - */
#define SE_KEY_PERIOD             46  /* . */
#define SE_KEY_SLASH              47  /* / */
#define SE_KEY_0                  48
#define SE_KEY_1                  49
#define SE_KEY_2                  50
#define SE_KEY_3                  51
#define SE_KEY_4                  52
#define SE_KEY_5                  53
#define SE_KEY_6                  54
#define SE_KEY_7                  55
#define SE_KEY_8                  56
#define SE_KEY_9                  57
#define SE_KEY_SEMICOLON          59  /* ; */
#define SE_KEY_EQUAL              61  /* = */
#define SE_KEY_A                  65
#define SE_KEY_B                  66
#define SE_KEY_C                  67
#define SE_KEY_D                  68
#define SE_KEY_E                  69
#define SE_KEY_F                  70
#define SE_KEY_G                  71
#define SE_KEY_H                  72
#define SE_KEY_I                  73
#define SE_KEY_J                  74
#define SE_KEY_K                  75
#define SE_KEY_L                  76
#define SE_KEY_M                  77
#define SE_KEY_N                  78
#define SE_KEY_O                  79
#define SE_KEY_P                  80
#define SE_KEY_Q                  81
#define SE_KEY_R                  82
#define SE_KEY_S                  83
#define SE_KEY_T                  84
#define SE_KEY_U                  85
#define SE_KEY_V                  86
#define SE_KEY_W                  87
#define SE_KEY_X                  88
#define SE_KEY_Y                  89
#define SE_KEY_Z                  90

#define SE_KEY_ESCAPE             256
#define SE_KEY_ENTER              257
#define SE_KEY_TAB                258
#define SE_KEY_BACKSPACE          259
#define SE_KEY_INSERT             260
#define SE_KEY_DELETE             261
#define SE_KEY_RIGHT              262
#define SE_KEY_LEFT               263
#define SE_KEY_DOWN               264
#define SE_KEY_UP                 265

#define SE_KEY_F1                 290
#define SE_KEY_F2                 291
#define SE_KEY_F3                 292
#define SE_KEY_F4                 293
#define SE_KEY_F5                 294
#define SE_KEY_F6                 295
#define SE_KEY_F7                 296
#define SE_KEY_F8                 297
#define SE_KEY_F9                 298
#define SE_KEY_F10                299
#define SE_KEY_F11                300

#define SE_KEY_LSHIFT			  340
#define SE_KEY_LCTRL			  341
#define SE_KEY_LALT				  342
#define SE_KEY_RSHIFT			  344
#define SE_KEY_RCTRL			  345
#define SE_KEY_RALT				  346

#define SE_MOUSE_BUTTON_LEFT      0
#define SE_MOUSE_BUTTON_RIGHT     1

 /********************************************************************************************
 * Graphics Related Functions
 * Initialise SEGraphics by using SEGfxInit()
 * Invoke SEGfxUpdate() in the beginning of every frame
 * Call SEGfxExit() upon exit. 
 *********************************************************************************************/

/**
	 * SEGraphics Init.
	 * @param width: Width of the window
	 * @param height: Height of the window
	 * @param fps: FPS Limit
	 * @param isEditor: set true if engine is used in Editor.
 */
void SEGfxInit(int width, int height, double fps, bool isEditor);

/**
	 * SEGraphics Update.
	 * To be called at the start of every frame.
 */
void SEGfxUpdate();

/**
	 * SEGraphics Exit.
	 * Memory clearing of all graphics and input calls.
 */
void SEGfxExit();

/**
	 * Render Font.
	 * @param text: String of characters to render on screen
	 * @param FontPackage: the font resource loaded. Retrieved using SEGetFont()
	 * @param x: X pos of the font to be rendered on screen
	 * @param y: Y pos of the font to be rendered on screen
	 * @param scale: scale of the font to be rendered on screen
	 * @param r g b: color of the font to be rendered on screen
 */
void SEDrawFont(std::string text, const SEFont& FontPackage, SEFloat x, SEFloat y, SEFloat scale, SEFloat r, SEFloat g, SEFloat b);


/**
	 * Render List.
	 * @param Lis: String of characters to render on screen
	 * @param Font: the font resource loaded. Retrieved using SEGetFont()
	 * @param pos: pos of the list to be rendered on screen
	 * @param Font_Scale: Desired font scale
	 * @param Line_Spacing: spacing between lines (0.0f - 1.0f)
	 * @param color: color of the font to be rendered on screen
 */
void SEDrawList(std::vector<std::string> List, const SEFont& Font, SEVec2 pos, SEFloat Font_Scale, SEFloat Line_Spacing, SEVec3 color);


/**
	 * Render a straight line.
	 * @param x0 y0: point A
	 * @param x1 y1: point B
	 * @param r g b: Color of the line
 */
void SEDrawLine(const SEFloat& x0, const SEFloat& y0, const SEFloat& x1, const SEFloat& y1, const SEFloat& r, const SEFloat& g, const SEFloat& b);

/**
	 * Render a point.
	 * @param x y: point on screen
	 * @param r g b: Color of the line
 */
void SEDrawPoint(const SEFloat& x, const SEFloat& y, const SEFloat& r, const SEFloat& g, const SEFloat& b);

/**
	 * Render a box.
	 * @param x y: middle point of the box 
	 * @param scale_x: scaling on the x axis of the box
	 * @param scale_y: scaling on the y axis of the box
	 * @param rot: angle of rotation of the box to be rendered. If there is not rotation, set to 0.
	 * @param r g b: Color of the line
 */
void SEDrawBox(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& rot, const SEFloat& r, const SEFloat& g, const SEFloat& b);

/**
	 * Render a quad, usually used for AABB. 
	 * @param minX minY: Minimum point of the quad
	 * @param maxX maxY: Maximum point of the quad
	 * @param r g b: Color of the box, usually 100 for checking collision aabb (red)
 */
void SEDrawQuad(const SEFloat& minX, const SEFloat& minY, const SEFloat& maxX, const SEFloat& maxY, const SEFloat& r, const SEFloat& g, const SEFloat& b);

/**
	 * Render a still texture on screen.
	 * @param x y: Position of the texture to be rendered on screen
	 * @param scale_x scale_y: Scaling of the texture
	 * @param angle: Angle of rotation of the texture. If there is no rotation needed, set it to 0.
	 * @param ID: Texture ID. Retrieved from SEGetTexture(texture path)
 */
void SEDrawTexture(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& angle, SEUint ID);

/*
	* Draws texture on screen
	* @param x: x pos 
	* @param y: y pos 
	* @param scale_x: scale x pos 
	* @param scale_y: scale y pos 
	* @param angle: angle of rotation of the texture rendered
	* @param uv: uv cordinates (BL -> BR -> TR -> TL) counter clockwise!
	* @param ID: texture ID. Retrieved using SEGetTexture()
*/
void SEDrawTexture(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& angle, const std::vector<SEVec2>& uv, SEUint ID);

void SEDrawSlider(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& angle, SEUint IDs);

/**
	* Checks if the window is closed.
 */
bool SECheckWindowIsClose();

/**
	* Call when beginning of the frame.
 */
void SEFrameStart();

/**
	* Call when end of the frame.
 */
void SEFrameEnd();

/**
	* Retrieve current FPS
 */
double SEGetFPS();

/**
	* Retrieve delta time.
 */
double SEGetDeltaTime();

// Added [2/2/2023]
void SESetDeltaTime(double dt);

/**
	* Render in full screen mode.
 */
void SESetFullScreen(SEBool flag);

/**
	* Checks if the screen is in full screen.
 */
bool SEIsFullScreen();

/**
	* Get current rendering window height.
 */
int SEGetWindowHeight();

/**
	 * Get current rendering window width.	
 */
int SEGetWindowWidth();

/**
	 * Set current camera.
	 * @param x y positon of the camera
 */
void SetCameraPosition(SEFloat x, SEFloat y);

/**
	* Set current camera height.
	* @param height Value to set for height
 */
void SESetCameraHeight(float height);

/**
	* Retrieve camera height.
 */
SEInt SEGetCameraHeight();

/**
	* Retrieve camera width.
 */
SEInt SEGetCameraWidth();

/**
	* Retrieve camera position on x axis.
 */
SEFloat GetCameraPositionX();

/**
	* Retrieve camera position on y axis.
 */
SEFloat GetCameraPositionY();

/**
	* Retrieve camera mat.
 */
glm::mat3 SEGetCameraMatrix();

/**
	* Retrieve World to NDC matrix
 */
glm::mat3 SEGetWorldToNDCMatrix();

glm::mat3 SEGetCameraProj(); // testing

/********************************************************************************************
	* Used for external frame buffers (eg IMGUI)
	* 
*********************************************************************************************/

/**
	 * Get external framebuffer. Used for imgui purposes.
 */
SEUint SEGetFrameBuffer();

/**
	* Get external texture buffer.
 */
SEUint SEGetTextureBuffer();

/**
	* Get current opengl render window.
 */
GLFWwindow* SEGetGLWindow();

/********************************************************************************************
	* Inputs checking
	*
*********************************************************************************************/

/**
*  
	* Get mouse position in screen space.
*/
glm::vec3 SEGetCursor();

/**
	* Get current cursor in world space.
 */
glm::vec3 SEGetCursorInWorld();

/**
	* Get mouse x position.
 */
double SEGetCursorX();

/**
	* Get mouse y position.
 */
double SEGetCursorY();

/**
	* Check if mouse cursor moves out of deadzone for zoom
 */
bool SECheckCursorDeadZone(SEVec2 CurrentPos, SEVec2 PreviousPos, int deadzone);

/**
	* Check if left mouse is clicked.
 */
SEBool SEIsMouseLeftPressed();

/**
 * Check if right mouse is clicked.
 */
SEBool SEIsMouseRightPressed();

/**
	* Check if left mouse is held.
 */
SEBool SEIsMouseLeftHeld();

/**
 * Check if right mouse is held.
 */
SEBool SEIsMouseRightHeld();

/**
 * Check Mouse Scroll value
 */
SEFloat SEMouseScroll();

/**
 * Resets Mouse Scroll value
 */
void ResetMouseScroll();

/**
	 * Check if a keyboard key is triggered.
	 * Usage: To pick up sth.
 */
SEBool SEIsKeyTriggered(SEInt key);

/**
	 * Check if a keyboard key is held down.
	 * Usage: To charge a skill or sth.
 */
SEBool SEIsKeyHeldDown(SEInt key);

/**
	 * Check if a keyboard key is released.
 */
SEBool SEIsKeyReleased(SEInt key);

/**
	 * Updates keystates at the end of frames
 */
void SEUpdateKeyStates();

// GLFW raw functions (used internally, users don't touch)
void SEKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void SEMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
void SEFrameBufferCB(GLFWwindow* window, int width, int height);

/**
	 * Check if a keyboard key is held down.
	 * Usage: To charge a skill or sth.
 */
GLuint resizeable_frameBuffer_Editor(int _width, int _height);

/**
	 * Set viewport height
	 * Usage: to adjust new height given by viewport by IMGUI 
 */
void SESetViewportHeight(float h);

SEVec2 SE_Bounded_Camera(SEVec2 _translate, SEVec2 _Cam_dimensions, SEVec4 _WorldBound);

inline bool showFPS = false;
void SESetWindowWidthHeight(int width, int height);