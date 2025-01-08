/******************************************************************************//**
 * \file			SEGraphics.cpp
 * \author 			Rommel Sim (100%)
 * \par    			zhenpengrommel.sim@digipen.edu
 *
 * \brief			Graphics system
 *
 * \date   			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#define _PARTICLE 0
#include "pch.h"
#include "SEGraphics.h"
#include "SEDebug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SEParticleSystem.h"
// Forward Declarations
class SEShader;
class SEGraphics;
std::unique_ptr<SEGraphics> SEGfx;

// Particle stuff
ParticleProps m_Particle;
ParticleSystem m_ParticleSystem;
float m_LastFrameTime;

class SEShader
{
    std::string		shader_log;
    GLuint			program_handler = 0;
    GLboolean		is_linked = GL_FALSE;
    enum ShaderType {
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER
    };
    GLint		GetUniformLocation(GLchar const* name);
    GLboolean	FileExists(std::string const& file_name);
public:
    SEShader();
    GLboolean	CompileLinkValidate(std::vector<std::pair<GLenum, std::string>>);
    GLboolean	CompileShaderFromFile(GLenum, const std::string&);
    GLboolean	CompileShaderFromString(GLenum, const std::string&);
    GLboolean	Link();
    GLboolean	Validate();
    GLuint		GetHandle() const;
    GLboolean	IsLinked() const;
    std::string GetLog() const;
    void		Use();
    void		UnUse();
    void		BindAtributeLocation(GLuint, const GLchar*);
    void		BindFragDataLocation(GLuint, const GLchar*);
    void		DeleteShaderPgm();
    void		SetUniform(GLchar const* name, GLboolean val);
    void		SetUniform(GLchar const* name, GLint val);
    void		SetUniform(GLchar const* name, GLfloat val);
    void		SetUniform(GLchar const* name, GLfloat x, GLfloat y);
    void		SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);
    void		SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void		SetUniform(GLchar const* name, glm::vec2 const& val);
    void		SetUniform(GLchar const* name, glm::vec3 const& val);
    void		SetUniform(GLchar const* name, glm::vec4 const& val);
    void		SetUniform(GLchar const* name, glm::mat3 const& val);
    void		SetUniform(GLchar const* name, glm::mat4 const& val);
    void		PrintActiveAttribs() const;
    void		PrintActiveUniforms() const;
};

class SEGraphics
{
public:
    bool m_isEditor;
    bool m_isFullScreen;
    glm::mat3 projection_transform;
    glm::mat3 model_to_ndc_transform;
    glm::mat3 world_to_ndc_transform;



    // Rendering
    using ShaderFiles = std::vector<std::pair<GLenum, std::string>>;
    SEShader			ShaderPgm_Color, ShaderPgm_Texture, ShaderPgm_Font;
    GLuint				VAO, VBO, EBO, VBO_Font;
    ShaderFiles			ShaderFiles_Color, ShaderFiles_Textured, ShaderFiles_Font;
    const std::string	VertexShader_Color = "./Assets/shaders/default_color.vert";		// changed to current directory. Reason is because installer and vs studio works difrenty.
    const std::string	VertexShader_Textured = "./Assets/shaders/default_textured.vert";
    const std::string	VertexShader_Font = "./Assets/shaders/default_font.vert";
    const std::string	FragmentShader_Color = "./Assets/shaders/default_color.frag";
    const std::string	FragmentShader_Textured = "./Assets/shaders/default_textured.frag";
    const std::string	FragmentShader_Font = "./Assets/shaders/default_font.frag";

    // Attribute data
    const GLuint Attribute_Vertex = 0;
    const GLuint Attribute_Color = 1;
    const GLuint Attribute_Texture = 2;
    const GLuint Attribute_Font = 3;

    // Binding Points
    const GLuint BindingPointA = 4;
    const GLuint BindingPointB = 5;
    const GLuint BindingPointC = 6;
    const GLuint BindingPointD = 7;
    const GLuint BindingPointE = 8;
    const GLuint BindingPointF = 9;
    const GLuint BindingPointG = 10;
    const GLuint BindingPointH = 11;

    std::vector<GLuint> Indices
    {
        0,1,3,
        1,2,3
    };

    // GLFW dat
    double				m_deltaTime;
    double				m_FPSLimit;
    int					m_window_width, m_window_height;
    double				m_currentFPS;
    std::string			m_title;
    GLFWwindow*         m_window;
    int                 winPos_x = 200, winPos_y = 200;
    int                 winSize_x, winSize_y;
    double              mouse_x, mouse_y;

    glm::mat3			m_view_transform;
    glm::mat3			m_projection_transform;
    glm::mat3			m_world_to_ndc_transform;

    bool                m_MouseButton_Left_isClicked;
    bool                m_MouseButton_Right_isClicked;

    bool                m_MouseButton_Left_isHeld;
    bool                m_MouseButton_Right_isHeld;

    float               m_MouseScroll_Offset_X;
    float               m_MouseScroll_Offset_Y;

    GLuint frameBuffer;
    GLuint frameBuffer_texture;

    SEGraphics(int window_width, int window_height, double fps);

    std::unordered_map<int, int> KeyboardCharacters_State;
    std::unordered_map<int, int> KeyboardCharacters_LastState;      // previous key state

    
    

    static void callback(int error, char const* description);
    static void fbcb(GLFWwindow* ptr_win, int _width, int _height);
    static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
public:
    class SECamera
    {
    public:
        enum class CAMERA_TYPE
        {
            FREE = 0,
            FIRST_PERSON = 1
        };
        CAMERA_TYPE m_ct = CAMERA_TYPE::FREE;
        glm::mat3 m_cam_view_xform;
        glm::vec3 m_right, m_up, m_center;
        float m_angle, m_rot_speed;
        float ar, m_height, m_width;
        int fb_width, fb_height;

        SECamera();
        SECamera(int x, int y);
        void Update();
        glm::mat3 SEGetCameraMatrix() { return m_cam_view_xform; }
    };

    std::unique_ptr<SECamera> Camera;
};


// Shader Defines

GLint SEShader::GetUniformLocation(GLchar const* name)
{
	return glGetUniformLocation(program_handler, name);
}

GLboolean SEShader::FileExists(std::string const& file_name)
{
	std::ifstream infile(file_name);
	return infile.good();
}

SEShader::SEShader() : program_handler(0), is_linked(GL_FALSE) {}

GLboolean SEShader::CompileLinkValidate(std::vector<std::pair<GLenum, std::string>> vec)
{
    for (auto& elem : vec) {
        if (GL_FALSE == CompileShaderFromFile(elem.first, elem.second.c_str())) {
#if _DEBUG
            std::cout << "Failed to compile shader \n";
            std::cout << shader_log << std::endl;
#else
            SEWrite("Failed to compile shader \n");
            SEWrite(shader_log);
            SEWrite("\n");
#endif
            exit(EXIT_FAILURE);
            return GL_FALSE;
        }
    }
    if (GL_FALSE == Link()) {
        return GL_FALSE;
    }
    if (GL_FALSE == Validate()) {
        return GL_FALSE;
    }
#if _DEBUG
    PrintActiveAttribs();
    PrintActiveUniforms();
#else
#endif
    return GL_TRUE;
}

GLboolean SEShader::CompileShaderFromFile(GLenum shader_type, const std::string& file_name)
{
    if (GL_FALSE == FileExists(file_name)) {
        shader_log = "File not found";
        return GL_FALSE;
    }
    if (program_handler <= 0) {
        program_handler = glCreateProgram();
        if (0 == program_handler) {
            shader_log = "Cannot create program handle";
            return GL_FALSE;
        }
    }

    std::ifstream shader_file(file_name, std::ifstream::in);
    if (!shader_file) {
        shader_log = "Error opening file " + file_name;
        return GL_FALSE;
    }
    std::stringstream buffer;
    buffer << shader_file.rdbuf();
    shader_file.close();
    return CompileShaderFromString(shader_type, buffer.str());
}

GLboolean SEShader::CompileShaderFromString(GLenum shader_type, const std::string& src)
{
    if (program_handler <= 0) {
        program_handler = glCreateProgram();
        if (0 == program_handler) {
            shader_log = "Cannot create program handle";
            return GL_FALSE;
        }
    }

    GLuint shader_handle = 0;
    switch (shader_type) {
    case VERTEX_SHADER: shader_handle = glCreateShader(GL_VERTEX_SHADER); break;
    case FRAGMENT_SHADER: shader_handle = glCreateShader(GL_FRAGMENT_SHADER); break;
    case GEOMETRY_SHADER: shader_handle = glCreateShader(GL_GEOMETRY_SHADER); break;
    case TESS_CONTROL_SHADER: shader_handle = glCreateShader(GL_TESS_CONTROL_SHADER); break;
    case TESS_EVALUATION_SHADER: shader_handle = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
    default:
        shader_log = "Incorrect shader type";
        return GL_FALSE;
    }

    // load shader source code into shader object
    GLchar const* shader_code[] = { src.c_str() };
    glShaderSource(shader_handle, 1, shader_code, NULL);

    // compile the shader
    glCompileShader(shader_handle);

    // check compilation status
    GLint comp_result;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &comp_result);
    if (GL_FALSE == comp_result) {
        shader_log = "Vertex shader compilation failed\n";
        GLint log_len;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 0) {
            GLchar* log = new GLchar[log_len];
            GLsizei written_log_len;
            glGetShaderInfoLog(shader_handle, log_len, &written_log_len, log);
            shader_log += log;
            delete[] log;
        }
        return GL_FALSE;
    }
    else { // attach the shader to the program object
        glAttachShader(program_handler, shader_handle);
        return GL_TRUE;
    }
}

GLboolean SEShader::Link()
{
    if (GL_TRUE == is_linked) {
        return GL_TRUE;
    }
    if (program_handler <= 0) {
        return GL_FALSE;
    }

    glLinkProgram(program_handler); // link the various compiled shaders

    // verify the link status
    GLint lnk_status;
    glGetProgramiv(program_handler, GL_LINK_STATUS, &lnk_status);
    if (GL_FALSE == lnk_status) {
        shader_log = "Failed to link shader program\n";
        GLint log_len;
        glGetProgramiv(program_handler, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 0) {
            GLchar* log_str = new GLchar[log_len];
            GLsizei written_log_len;
            glGetProgramInfoLog(program_handler, log_len, &written_log_len, log_str);
            shader_log += log_str;
            delete[] log_str;
        }
        return GL_FALSE;
    }
    return is_linked = GL_TRUE;
}

GLboolean SEShader::Validate()
{
    if (program_handler <= 0 || is_linked == GL_FALSE) {
        return GL_FALSE;
    }

    glValidateProgram(program_handler);
    GLint status;
    glGetProgramiv(program_handler, GL_VALIDATE_STATUS, &status);
    if (GL_FALSE == status) {
        shader_log = "Failed to validate shader program for current OpenGL context\n";
        GLint log_len;
        glGetProgramiv(program_handler, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 0) {
            GLchar* log_str = new GLchar[log_len];
            GLsizei written_log_len;
            glGetProgramInfoLog(program_handler, log_len, &written_log_len, log_str);
            shader_log += log_str;
            delete[] log_str;
        }
        return GL_FALSE;
    }
    else {
        return GL_TRUE;
    }
}

GLuint SEShader::GetHandle() const
{
    return program_handler;
}

GLboolean SEShader::IsLinked() const
{
    return is_linked;
}

std::string SEShader::GetLog() const
{
    return shader_log;
}

void SEShader::Use()
{
    glUseProgram(program_handler);
}

void SEShader::UnUse()
{
    glUseProgram(0);
}

void SEShader::BindAtributeLocation(GLuint idx, const GLchar* name)
{
    glBindAttribLocation(program_handler, idx, name);
}

void SEShader::BindFragDataLocation(GLuint color_num, const GLchar* name)
{
    glBindFragDataLocation(program_handler, color_num, name);
}

void SEShader::DeleteShaderPgm()
{
    glDeleteShader(program_handler);
}

void SEShader::SetUniform(GLchar const* name, GLboolean val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform1i(loc, val);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, GLint val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform1i(loc, val);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, GLfloat val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform1f(loc, val);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, GLfloat x, GLfloat y)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform2f(loc, x, y);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform3f(loc, x, y, z);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform4f(loc, x, y, z, w);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, glm::vec2 const& val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform2f(loc, val.x, val.y);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, glm::vec3 const& val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform3f(loc, val.x, val.y, val.z);
    }
    else {
        // uColor Missing Error Here!
       // std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
    }
}

void SEShader::SetUniform(GLchar const* name, glm::vec4 const& val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniform4f(loc, val.x, val.y, val.z, val.w);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, glm::mat3 const& val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::SetUniform(GLchar const* name, glm::mat4 const& val)
{
    GLint loc = glGetUniformLocation(program_handler, name);
    if (loc >= 0) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
    }
    else {
#if _DEBUG
        std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
#endif
    }
}

void SEShader::PrintActiveAttribs() const
{
    GLint max_length, num_attribs;
    glGetProgramiv(program_handler, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
    glGetProgramiv(program_handler, GL_ACTIVE_ATTRIBUTES, &num_attribs);
    GLchar* pname = new GLchar[max_length];
#if _DEBUG
    std::cout << "Index\t|\tName\n";
    std::cout << "----------------------------------------------------------------------\n";
#endif
    for (GLint i = 0; i < num_attribs; ++i) {
        GLsizei written;
        GLint size;
        GLenum type;
        glGetActiveAttrib(program_handler, i, max_length, &written, &size, &type, pname);
        GLint loc = glGetAttribLocation(program_handler, pname); (void)loc;
#if _DEBUG
        std::cout << loc << "\t\t" << pname << std::endl;
#endif
    }
#if _DEBUG
    std::cout << "----------------------------------------------------------------------\n";
#endif
    delete[] pname;
}

void SEShader::PrintActiveUniforms() const
{
    GLint max_length;
    glGetProgramiv(program_handler, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
    GLchar* pname = new GLchar[max_length];
    GLint num_uniforms;
    glGetProgramiv(program_handler, GL_ACTIVE_UNIFORMS, &num_uniforms);
#if _DEBUG
    std::cout << "Location\t|\tName\n";
    std::cout << "----------------------------------------------------------------------\n";
#endif
    for (GLint i = 0; i < num_uniforms; ++i) {
        GLsizei written;
        GLint size;
        GLenum type;
        glGetActiveUniform(program_handler, i, max_length, &written, &size, &type, pname);
        GLint loc = glGetUniformLocation(program_handler, pname); (void)loc;
#if _DEBUG
        std::cout << loc << "\t\t" << pname << std::endl;
#endif
    }
#if _DEBUG
    std::cout << "----------------------------------------------------------------------\n";
#endif
    delete[] pname;
}

// Graphics SEGfx Defines

SEGraphics::SEGraphics(int window_width, int window_height, double fps) : m_window_width(window_width), m_window_height(window_height), m_FPSLimit(fps)
{
    if (!glfwInit())
    {
#if _DEBUG
        std::cout << "glfw fail to init\n Aborting now.";
#endif
        exit(EXIT_FAILURE);
    }

    // In case a GLFW function fails, an error is reported to callback function
    glfwSetErrorCallback(callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

    //m_window = glfwCreateWindow(m_window_width, m_window_height, m_title.c_str(), glfwGetPrimaryMonitor(), 0);
    m_window = glfwCreateWindow(m_window_width, m_window_height, m_title.c_str(), 0, 0);
    if (!m_window)
    {
#if _DEBUG
        std::cout << "GLFW unable to create OpenGL context - abort program\n";
#else
        SEWrite("GLFW unable to create OpenGL context - abort program\n");
#endif
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, fbcb);
    glfwSetScrollCallback(m_window, mousescroll_cb);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
#if _DEBUG
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
#else
        SEWrite("Unable to initialize GLEW. abort program\n");
#endif
        exit(EXIT_FAILURE);
    }
    if (GLEW_VERSION_4_5) {
#if _DEBUG
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
#else
        SEWrite("Driver supports OpenGL 4.5\n");
#endif
    }
    else {
#if _DEBUG
        std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
#else
        SEWrite("Driver doesn't supports OpenGL 4.5 - abort program\n");
        exit(EXIT_FAILURE);
#endif
    }
#if _DEBUG
    std::cout << "Graphics initialised!\n";
#else
    SEWrite("Graphics initialised!\n");
#endif		

}

void SEGraphics::callback(int error, char const* description)
{
    (void)description; (void)error;
#if _DEBUG
    //std::cerr << "GLFW error: " << "(" << error << ")" << description << std::endl;
#endif
}

void SEGraphics::fbcb(GLFWwindow* ptr_win, int _width, int _height)
{
    (void)ptr_win;
#if _DEBUG
    std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
    glViewport(0, 0, _width, _height);
}

void SEGraphics::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset)
{
    SEGfx->m_MouseScroll_Offset_X = (float)xoffset;
    SEGfx->m_MouseScroll_Offset_Y = (float)yoffset;
    (void)pwin;
#if _DEBUG
    std::cout << "Mouse scroll wheel offset: ("
        << xoffset << ", " << yoffset << ")" << std::endl;
#endif
}

void SEGraphics::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)xpos; (void)ypos; (void)window;
    glfwGetCursorPos(SEGfx->m_window, &SEGfx->mouse_x, &SEGfx->mouse_y);
#if _DEBUG
    PRINT("Mouse Position: (%.2f, %.2f)\n", SEGfx->mouse_x, SEGfx->mouse_y);
#endif
}

void SEGraphics::window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    (void)window;
    if (xpos == 0 || ypos == 0)
    {
        std::cout << "ass\n";
        xpos = SEGfx->winPos_x;
        ypos = SEGfx->winPos_y;
    }
    else
    {
        glfwGetWindowPos(SEGfx->m_window, &xpos, &ypos);
        SEGfx->winPos_x = xpos;
        SEGfx->winPos_y = ypos;
    }   
}

void SEGraphics::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    (void)mods; (void)action; (void)button; (void)window;
}


// Graphics Camera Defines

SEGraphics::SECamera::SECamera() : m_cam_view_xform{}, m_right{}, m_up{}, m_center{ 0,0,1 }, m_angle{},
m_rot_speed{}, ar{}, m_height{ 1000 }, m_width{}, fb_width{}, fb_height{}
{
    glfwGetFramebufferSize(SEGfx->m_window, &fb_width, &fb_height);
    ar = (float)(fb_width) / (float)(fb_height);
    m_width = ar * m_height;
#if _DEBUG
    std::cout << "Cam init!\n";
#endif
}

SEGraphics::SECamera::SECamera(int x, int y): m_cam_view_xform{}, m_right{}, m_up{}, m_center{ x,y,1 },
m_angle{}, m_rot_speed{}, ar{}, m_height{ 1000 }, m_width{}, fb_width{}, fb_height{}
{
    glfwGetFramebufferSize(SEGfx->m_window, &fb_width, &fb_height);
    ar = (float)(fb_width) / (float)(fb_height);
    m_width = ar * m_height;
#if _DEBUG
    std::cout << "Cam init!\n";
#endif
}

void SEGraphics::SECamera::Update()
{
    m_right = glm::vec3
    (
        glm::cos(glm::radians(m_angle)),
        glm::sin(glm::radians(m_angle)),
        1
    );
    m_up = glm::vec3
    (
        -glm::sin(glm::radians(m_angle)),
        glm::cos(glm::radians(m_angle)),
        1
    );
    switch (m_ct)
    {
    case CAMERA_TYPE::FREE:
        m_cam_view_xform = glm::mat3
        (
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(-m_center.x, -m_center.y, 1)
        );
        break;
    case CAMERA_TYPE::FIRST_PERSON:
        m_cam_view_xform = glm::mat3
        (
            glm::vec3(m_right.x, m_up.x, 0),
            glm::vec3(m_right.y, m_up.y, 0),
            glm::vec3(glm::dot(-m_right, m_center), glm::dot(-m_up, m_center), 1)
        );
        break;
    default:
        break;
    }

    // update cam zoom in and out effect
     m_width = ar * m_height;
}

// Header defines
GLuint resizeable_frameBuffer_Editor(int _width, int _height)
{

    //delete; 
    if (SEGfx->frameBuffer != 0)
    {
        glDeleteFramebuffers(1, &SEGfx->frameBuffer);
        SEGfx->frameBuffer = 0;
        glDeleteTextures(1, &SEGfx->frameBuffer_texture);

        glGenFramebuffers(1, &SEGfx->frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    #if _DEBUG
        std::cout << "Frame Buffer FBO: " << SEGfx->frameBuffer << std::endl;
    #endif
        glGenTextures(1, &SEGfx->frameBuffer_texture);
        glBindTexture(GL_TEXTURE_2D, SEGfx->frameBuffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SEGfx->frameBuffer_texture, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
    #if _DEBUG
            std::cout << "Error creating FBO\n";
    #endif
        }
        else
        {
    #if _DEBUG
            std::cout << "created screen buffer width: " << _width << " height: " << _height << std::endl;
    #endif
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    glViewport(0, 0, _width, _height);
    return SEGfx->frameBuffer;
}
void SESetViewportHeight(float h)
{
    SEGfx->Camera->m_height = h;
}
void SEGfxInit(int width, int height, double fps, bool isEditor)
{
    SEGfx = std::make_unique<SEGraphics>(width, height, fps);
    SEGfx->m_isEditor = isEditor;
    glfwGetWindowPos(SEGfx->m_window, &SEGfx->winPos_x, &SEGfx->winPos_y);
    glfwSetMouseButtonCallback(SEGfx->m_window, SEMouseButtonCallBack);
    glfwSetKeyCallback(SEGfx->m_window, SEKeyCallBack);
    glfwSetFramebufferSizeCallback(SEGfx->m_window, SEFrameBufferCB);

    // from A -> ESC
    for (int i = 0; i <= 350; ++i)
    {
        //SEGfx->KeyboardCharacters_Triggered[i] = 0;
        //SEGfx->KeyboardCharacters_Held[i] = 0;
        //SEGfx->KeyboardCharacters_Released[i] = 0;
        SEGfx->KeyboardCharacters_LastState[i] = GLFW_RELEASE;
        SEGfx->KeyboardCharacters_State[i] = GLFW_RELEASE;
    }

    glGenFramebuffers(1, &SEGfx->frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
#if _DEBUG
    std::cout << "Frame Buffer FBO: " << SEGfx->frameBuffer << std::endl;
#endif
    glGenTextures(1, &SEGfx->frameBuffer_texture);
    glBindTexture(GL_TEXTURE_2D, SEGfx->frameBuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SEGfx->m_window_width, SEGfx->m_window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SEGfx->frameBuffer_texture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
#if _DEBUG
        std::cout << "Error creating FBO\n";
#endif
    }
    else
    {
#if _DEBUG
        std::cout << "created screen buffer width: " << SEGfx->m_window_width << " height: " << SEGfx->m_window_height << std::endl;
#endif
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup ShaderProgram for Color Fragment
    SEGfx->ShaderFiles_Color.push_back(std::make_pair(GL_VERTEX_SHADER, SEGfx->VertexShader_Color));
    SEGfx->ShaderFiles_Color.push_back(std::make_pair(GL_FRAGMENT_SHADER, SEGfx->FragmentShader_Color));
    SEGfx->ShaderPgm_Color.CompileLinkValidate(SEGfx->ShaderFiles_Color);

    // Setup ShaderProgram for Textured Fragment
    SEGfx->ShaderFiles_Textured.push_back(std::make_pair(GL_VERTEX_SHADER, SEGfx->VertexShader_Textured));
    SEGfx->ShaderFiles_Textured.push_back(std::make_pair(GL_FRAGMENT_SHADER, SEGfx->FragmentShader_Textured));
    SEGfx->ShaderPgm_Texture.CompileLinkValidate(SEGfx->ShaderFiles_Textured);

    // Setup ShaderProgram for Fonts
    SEGfx->ShaderFiles_Font.push_back(std::make_pair(GL_VERTEX_SHADER, SEGfx->VertexShader_Font));
    SEGfx->ShaderFiles_Font.push_back(std::make_pair(GL_FRAGMENT_SHADER, SEGfx->FragmentShader_Font));
    SEGfx->ShaderPgm_Font.CompileLinkValidate(SEGfx->ShaderFiles_Font);

    // Setup VBO and friends
    glCreateBuffers(1, &SEGfx->VBO);
    glNamedBufferStorage(SEGfx->VBO, sizeof(glm::vec3) * 8 + sizeof(glm::vec3) * 8 + sizeof(glm::vec3) * 8, 0, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &SEGfx->VBO_Font);
    glNamedBufferStorage(SEGfx->VBO_Font, sizeof(float) * 6 * 4, 0, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &SEGfx->VAO);
    glEnableVertexArrayAttrib(SEGfx->VAO, SEGfx->Attribute_Vertex);
    glEnableVertexArrayAttrib(SEGfx->VAO, SEGfx->Attribute_Color);
    glEnableVertexArrayAttrib(SEGfx->VAO, SEGfx->Attribute_Texture);
    glEnableVertexArrayAttrib(SEGfx->VAO, SEGfx->Attribute_Font);

    // [V] = [Vertex]
    // [C] = [Color]
    // [T] = [Texture]
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointA, SEGfx->VBO, 0, sizeof(glm::vec3));												// [V]
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointB, SEGfx->VBO, sizeof(glm::vec3), sizeof(glm::vec3));								// [V][C]
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointC, SEGfx->VBO, sizeof(glm::vec3) * 2, sizeof(glm::vec3));							// [V][V][C][C]
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointD, SEGfx->VBO, sizeof(glm::vec3) * 4, sizeof(glm::vec3));							// [V][V][V][V][C][C][C][C]
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointE, SEGfx->VBO, sizeof(glm::vec3) * 4 + sizeof(glm::vec3) * 4, sizeof(glm::vec2));	// [V][V][V][V][C][C][C][C][T][T][T][T]
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointF, SEGfx->VBO, sizeof(glm::vec3) * 3, sizeof(glm::vec3));							// [V][V][V][C][C][C]
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointG, SEGfx->VBO_Font, 0, sizeof(float) * 4);
    glVertexArrayVertexBuffer(SEGfx->VAO, SEGfx->BindingPointH, SEGfx->VBO, sizeof(glm::vec3) * 8, sizeof(glm::vec3));

    glVertexArrayAttribFormat(SEGfx->VAO, SEGfx->Attribute_Vertex, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(SEGfx->VAO, SEGfx->Attribute_Color, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(SEGfx->VAO, SEGfx->Attribute_Texture, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(SEGfx->VAO, SEGfx->Attribute_Font, 4, GL_FLOAT, GL_FALSE, 0);

    glCreateBuffers(1, &SEGfx->EBO);
    glNamedBufferStorage(SEGfx->EBO, sizeof(GLuint) * 6, SEGfx->Indices.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(SEGfx->VAO, SEGfx->EBO);

   stbi_set_flip_vertically_on_load(true); 
    SEGfx->Camera = std::make_unique<SEGraphics::SECamera>();

#if _DEBUG
       std::cout << "Graphic System Successfully Initialised!\n";
#else
    SEWrite("Graphic System Successfully Initialised!\n");
#endif

    // Particle Init here
    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin = 30.0f, m_Particle.SizeVariation = 10.0f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position = { 0.0f, 0.0f };
}

void SEGfxUpdate()
{
    // Check for user inputs on the opengl Window, NOT imgui!
    if (SEGfx)
    {
        // Cursor Position
        glfwGetCursorPos(SEGetGLWindow(), &SEGfx->mouse_x, &SEGfx->mouse_y);
        
        SEUpdateKeyStates();
        // Reset all mouse and key states at the start of the frame.
        //for (int i = 0; i <= 350; ++i)
        //{

        //    SEGfx->KeyboardCharacters_Triggered[i] = 0;
        //    if (SEGfx->KeyboardCharacters_Held[i] == GLFW_RELEASE)
        //        SEGfx->KeyboardCharacters_Held[i] = 0;
        //    SEGfx->KeyboardCharacters_Released[i] = 0;
        //}


      
    }

   
    if (SEGfx && SEGfx->m_isEditor)
    {
        // cleqr external frame buffer!
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
 
#if _PARTICLE
    // particle stuff
    if (glfwGetMouseButton(SEGetGLWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    //if(SEIsMouseLeftPressed())
    {
        std::cout << "lmao" << "\n";
        float time = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        m_Particle.Position = glm::vec2{ SEGetCursorInWorld().x, SEGetCursorInWorld().y };

        for (int i = 0; i < 5; i++)
        {
            m_ParticleSystem.Emit(m_Particle);
            m_ParticleSystem.onUpdate(timestep);
            m_ParticleSystem.onRender();
        }
    }
#endif

    static float fpsDelay = 1.0;
    static float fps = (float)SEGfx->m_currentFPS;
    if (showFPS)
    {
        char fpstext[20];
        if (fpsDelay < 0)
        {
            fpsDelay = 0.25;
            fps = (float)SEGfx->m_currentFPS;
        }
        else
        {
            fpsDelay -= (float)SEGetDeltaTime();
        }
        sprintf_s(fpstext, sizeof(fpstext), R"(FPS: %.2f)", fps);
        SEDrawFont(fpstext, SEGetFont("Assets/Font/IndieFlower.ttf"),
        GetCameraPositionX() - 825, GetCameraPositionY() + 400, 100.0f, 1.0f, 0.0f, 0.0f);
    }
    
   //glfwSetKeyCallback(SEGfx->m_window, SEGfx->key_callback);
   //glfwSetCursorPosCallback(SEGfx->m_window, SEGfx->cursor_position_callback);
   //glfwSetMouseButtonCallback(SEGfx->m_window, SEGfx->mouse_button_callback);
    
    glfwSwapBuffers(SEGfx->m_window);
    glfwPollEvents();

    //// Triggered -> Held -> Released
    //for (int i = 0; i <= 350; ++i)
    //{
    //    if (SEGfx->KeyboardCharacters_LastState[i] != SEGfx->KeyboardCharacters_Held[i])
    //    {
    //        // NON ACTIVE -> ACTIVE
    //        if (SEGfx->KeyboardCharacters_Held[i] == false)
    //        {
    //            SEGfx->KeyboardCharacters_Released[i] = true;
    //        }
    //        else
    //        {
    //            SEGfx->KeyboardCharacters_Triggered[i] = true;
    //        }
    //    }
    //}



#if _DEBUG
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "StormEngine " << SEGfx->m_currentFPS;
    glfwSetWindowTitle(SEGfx->m_window, ss.str().c_str());
#endif
   
    SEGfx->Camera->Update();

    SEGfx->m_view_transform = SEGfx->Camera->m_cam_view_xform;

    // update proj
    SEGfx->m_projection_transform = glm::mat3
    (
        glm::vec3((float)2.0f / (float)SEGfx->Camera->m_width, 0, 0),
        glm::vec3(0, (float)2.0f / (float)SEGfx->Camera->m_height, 0),
        glm::vec3(0, 0, 1)
    );

    SEGfx->m_world_to_ndc_transform = SEGfx->m_projection_transform * SEGfx->m_view_transform;
    glClear(GL_COLOR_BUFFER_BIT);			// clear original frame buffer
}

void SEGfxExit()
{
    if (SEGfx)
    {
        glfwDestroyWindow(SEGfx->m_window);
        glfwTerminate();
        SEWrite("GFX terminate successfully\n");
    }

}

// DrawFont
// [1] Text to draw.
// [2] Font Package (getFont() can already)
// [3] Transform.x 
// [4] Transform.y 
// [5] Transform.scale
// [6] Color.r 
// [7] Color.g
// [8] Color.b

void SEDrawFont(std::string text, const SEFont& FontPackage, SEFloat x, SEFloat y, SEFloat scale, SEFloat r, SEFloat g, SEFloat b)
{
 /*   glm::mat3 translate =
    {
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(x, y , 1)
    };*/
    glm::mat4 projection = SEGfx->m_world_to_ndc_transform;// *translate;
    scale = scale / 100.0f;

    //glm::mat4 projection = glm::ortho(-static_cast<float>(m_window_width), static_cast<float>(m_window_width), -static_cast<float>(m_window_height), static_cast<float>(m_window_height));


    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); ++c)
    {
        if (SEGfx && SEGfx->m_isEditor)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glEnable(GL_TEXTURE_2D);
            glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
        }
        //glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        SEGfx->ShaderPgm_Font.Use();

        glUniformMatrix4fv(glGetUniformLocation(SEGfx->ShaderPgm_Font.GetHandle(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(SEGfx->ShaderPgm_Font.GetHandle(), "textColor"), r, g, b);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(SEGfx->VAO);

        //SE_Font::Character ch = FontPackage.at(*c);
        SECharacter ch = FontPackage.at(*c);
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Font, SEGfx->BindingPointG);
        glNamedBufferSubData(SEGfx->VBO_Font, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;

        if (SEGfx && SEGfx->m_isEditor)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        SEGfx->ShaderPgm_Font.UnUse();

    }
}

void SEDrawList(std::vector<std::string> List, const SEFont& Font, SEVec2 pos, SEFloat Font_Scale, SEFloat Line_Spacing, SEVec3 color)
{
    SEVec2 last_member_pos = pos;
    Line_Spacing = Line_Spacing * 0.01f;

    for (auto& list_mem : List) {
        SEDrawFont(list_mem, Font, last_member_pos.x, last_member_pos.y, Font_Scale, color.r, color.g, color.b);

        last_member_pos.y += (Font_Scale + (Font_Scale * Line_Spacing));
    }
}

void SEDrawLine(const SEFloat& x0, const SEFloat& y0, const SEFloat& x1, const SEFloat& y1, const SEFloat& r, const SEFloat& g, const SEFloat& b)
{
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    }
    
    glm::mat3 mat_trans0 = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x0, y0, 1)
    );

    glm::mat3 mat_trans1 = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x1, y1, 1)
    );

    glm::mat3 mdl_xform0 = mat_trans0;
    glm::mat3 mdl_xform1 = mat_trans1;
    glm::mat3 mdl_to_ndc_xform0 = SEGfx->m_world_to_ndc_transform * mdl_xform0;
    glm::mat3 mdl_to_ndc_xform1 = SEGfx->m_world_to_ndc_transform * mdl_xform1;

    glm::vec3 p0{ x0, y0, 1 };
    glm::vec3 p1{ x1, y1, 1 };
    p0 = mdl_to_ndc_xform0 * p0;
    p1 = mdl_to_ndc_xform1 * p1;
    std::vector<glm::vec3> points{ p0,p1 };
    std::vector<glm::vec3> colors{ {r,g,b}, {r,g,b} };

    glBindVertexArray(SEGfx->VAO);
    SEGfx->ShaderPgm_Color.Use();
    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Vertex, SEGfx->BindingPointA);
    glNamedBufferSubData(SEGfx->VBO, 0, sizeof(glm::vec3) * 2, points.data());

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Color, SEGfx->BindingPointC);
    glNamedBufferSubData(SEGfx->VBO, sizeof(glm::vec3) * 2, sizeof(glm::vec3) * 2, colors.data());

    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    SEGfx->ShaderPgm_Color.UnUse();
}

void SEDrawPoint(const SEFloat& x, const SEFloat& y, const SEFloat& r, const SEFloat& g, const SEFloat& b)
{
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    }
    glm::mat3 mat_scale, mat_rot, mat_trans;
    mat_scale = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1)
    );
    float deg = 0.0f;
    mat_rot = glm::mat3
    (
        glm::vec3(glm::cos(glm::radians(deg)), glm::sin(glm::radians(deg)), 0),
        glm::vec3(-glm::sin(glm::radians(deg)), glm::cos(glm::radians(deg)), 0),
        glm::vec3(0, 0, 1)
    );
    mat_trans = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x, y, 1)
    );
    glm::mat3 mdl_xform = mat_trans * (mat_rot * mat_scale);
    glm::mat3 mdl_to_ndc_xform = SEGfx->m_world_to_ndc_transform * mdl_xform;
    glm::vec3 p0 = glm::vec3(-1, -1, 1);
    p0 = mdl_to_ndc_xform * p0;
    std::vector<glm::vec3> points{ p0 };
    //glm::vec3 c{ clr };
    std::vector<glm::vec3> colors{ {r,g,b} };

    glBindVertexArray(SEGfx->VAO);
    SEGfx->ShaderPgm_Color.Use();
    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Vertex, SEGfx->BindingPointA);
    glNamedBufferSubData(SEGfx->VBO, 0, sizeof(glm::vec3), points.data());
    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Color, SEGfx->BindingPointB);
    glNamedBufferSubData(SEGfx->VBO, sizeof(glm::vec3), sizeof(glm::vec3), colors.data());

    glDrawArrays(GL_POINTS, 0, 1);

    glBindVertexArray(0);
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    SEGfx->ShaderPgm_Color.UnUse();
}

void SEDrawBox(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& rot, const SEFloat& r, const SEFloat& g, const SEFloat& b)
{
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    }
    glm::mat3 mat_scale, mat_rot, mat_trans;
    mat_scale = glm::mat3
    (
        glm::vec3(scale_x, 0, 0),
        glm::vec3(0, scale_y, 0),
        glm::vec3(0, 0, 1)
    );
    float deg = rot;
    mat_rot = glm::mat3
    (
        glm::vec3(glm::cos(glm::radians(deg)), glm::sin(glm::radians(deg)), 0),
        glm::vec3(-glm::sin(glm::radians(deg)), glm::cos(glm::radians(deg)), 0),
        glm::vec3(0, 0, 1)
    );
    mat_trans = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x, y, 1)
    );

    glm::mat3 mdl_xform = mat_trans * (mat_rot * mat_scale);
    glm::mat3 mdl_to_ndc_xform = SEGfx->m_world_to_ndc_transform * mdl_xform;
    glm::vec3 p0 = glm::vec3(-1, -1, 1);
    glm::vec3 p1 = glm::vec3(1, -1, 1);
    glm::vec3 p2 = glm::vec3(1, 1, 1);
    glm::vec3 p3 = glm::vec3(-1, 1, 1);

    p0 = mdl_to_ndc_xform * p0;
    p1 = mdl_to_ndc_xform * p1;
    p2 = mdl_to_ndc_xform * p2;
    p3 = mdl_to_ndc_xform * p3;
    std::vector<glm::vec3> points{ p0,p1,p2,p3 };

    glm::vec3 c{ r,g,b };
    std::vector<glm::vec3> colors{ c,c,c,c };

    glBindVertexArray(SEGfx->VAO);
    SEGfx->ShaderPgm_Color.Use();

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Vertex, SEGfx->BindingPointA);
    glNamedBufferSubData(SEGfx->VBO, 0, sizeof(glm::vec3) * 4, points.data());

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Color, SEGfx->BindingPointD);
    glNamedBufferSubData(SEGfx->VBO, sizeof(glm::vec3) * 4, sizeof(glm::vec3) * 4, colors.data());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    SEGfx->ShaderPgm_Color.UnUse();
}

void SEDrawQuad(const SEFloat& minX, const SEFloat& minY, const SEFloat& maxX, const SEFloat& maxY, const SEFloat& r, const SEFloat& g, const SEFloat& b)
{
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    }
    glm::vec3 p0{ minX, minY, 1 };
    glm::vec3 p1{ maxX, minY, 1 };
    glm::vec3 p2{ maxX, maxY, 1 };
    glm::vec3 p3{ minX, maxY, 1 };

    p0 = SEGfx->m_world_to_ndc_transform * p0;
    p1 = SEGfx->m_world_to_ndc_transform * p1;
    p2 = SEGfx->m_world_to_ndc_transform * p2;
    p3 = SEGfx->m_world_to_ndc_transform * p3;

    glm::vec3 clr{ r,g,b };
    std::vector<glm::vec3> points{ p0,p1, p1,p2, p2,p3, p3,p0 };
    std::vector<glm::vec3> colors{ clr,clr,clr,clr,clr,clr,clr,clr };

    glBindVertexArray(SEGfx->VAO);
    SEGfx->ShaderPgm_Color.Use();

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Vertex, SEGfx->BindingPointA);
    glNamedBufferSubData(SEGfx->VBO, 0, sizeof(glm::vec3) * 8, points.data());

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Color, SEGfx->BindingPointH);
    glNamedBufferSubData(SEGfx->VBO, sizeof(glm::vec3) * 8, sizeof(glm::vec3) * 8, colors.data());

    glDrawArrays(GL_LINES, 0, 8);

    glBindVertexArray(0); // crashed here
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    SEGfx->ShaderPgm_Color.UnUse();
}

void SEDrawTexture(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& angle, SEUint ID)
{
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    }
    glm::mat3 mat_scale = glm::mat3
    (
        glm::vec3(scale_x, 0, 0),
        glm::vec3(0, scale_y, 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_rot = glm::mat3
    (
        glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0),
        glm::vec3(-glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_trans = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x, y, 1)
    );

    glm::mat3 mdl = mat_trans * (mat_rot * mat_scale);
    glm::mat3 mdl_to_ndc = SEGfx->m_world_to_ndc_transform * mdl;

    glm::vec3 p0{ -1,-1,1 };
    glm::vec3 p1{ 1,-1,1 };
    glm::vec3 p2{ 1, 1,1 };
    glm::vec3 p3{ -1, 1,1 };
    p0 = mdl_to_ndc * p0;
    p1 = mdl_to_ndc * p1;
    p2 = mdl_to_ndc * p2;
    p3 = mdl_to_ndc * p3;

    glm::vec2 uv0{ 0,0 };
    glm::vec2 uv1{ 1,0 };
    glm::vec2 uv2{ 1,1 };
    glm::vec2 uv3{ 0,1 };

    std::vector<glm::vec3> points{ p0,p1,p2,p3 };
    std::vector<glm::vec2> uv{ uv0, uv1,uv2,uv3 };
    glBindVertexArray(SEGfx->VAO);
    SEGfx->ShaderPgm_Texture.Use();

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Vertex, SEGfx->BindingPointA);
    glNamedBufferSubData(SEGfx->VBO, 0, sizeof(glm::vec3) * 4, points.data());

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Texture, SEGfx->BindingPointE);
    glNamedBufferSubData(SEGfx->VBO, sizeof(glm::vec3) * 4 + sizeof(glm::vec3) * 4, sizeof(glm::vec2) * 4, uv.data());

    glBindTextureUnit(0, ID);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, ID);
    GLuint loc = glGetUniformLocation(SEGfx->ShaderPgm_Texture.GetHandle(), "uTex2D");
    glUniform1i(loc, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glBindVertexArray(0);
    SEGfx->ShaderPgm_Texture.UnUse();
}

void SEDrawSlider(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& angle, SEUint ID)
{
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    }

    glm::vec3 p0{ -1,-1,1 };
    glm::vec3 p1{ 1,-1,1 };
    glm::vec3 p2{ 1, 1,1 };
    glm::vec3 p3{ -1, 1,1 };

    // Only need to update p1, p2
    glm::mat3 mat_scale_p1_p2 = glm::mat3
    (
        glm::vec3(scale_x, 0, 0),
        glm::vec3(0, scale_y, 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_rot = glm::mat3
    (
        glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0),
        glm::vec3(-glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_trans = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x, y, 1)
    );

    glm::mat3 mdl_p1_p2 = mat_trans * (mat_rot * mat_scale_p1_p2);
    glm::mat3 mdl_p1_p2_to_ndc = SEGfx->m_world_to_ndc_transform * mdl_p1_p2;
    p1 = mdl_p1_p2_to_ndc * p1;
    p2 = mdl_p1_p2_to_ndc * p2;


    // Dont update p0 and p3 scale_x 
    glm::mat3 mat_scale_p0_p3 = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, scale_y, 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mdl_p0_p3 = mat_trans * (mat_rot * mat_scale_p0_p3);
    glm::mat3 mdl_p0_p3_to_ndc = SEGfx->m_world_to_ndc_transform * mdl_p0_p3;
    p0 = mdl_p0_p3_to_ndc * p0;
    p3 = mdl_p0_p3_to_ndc * p3;

    glm::vec2 uv0{ 0,0 };
    glm::vec2 uv1{ 1,0 };
    glm::vec2 uv2{ 1,1 };
    glm::vec2 uv3{ 0,1 };
    std::vector<glm::vec2> uv{ uv0, uv1,uv2,uv3 };

    std::vector<glm::vec3> points{ p0,p1,p2,p3 };


    /*glm::mat3 mat_scale = glm::mat3
    (
        glm::vec3(scale_x, 0, 0),
        glm::vec3(0, scale_y, 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_rot = glm::mat3
    (
        glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0),
        glm::vec3(-glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_trans = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x, y, 1)
    );

    glm::mat3 mdl = mat_trans * (mat_rot * mat_scale);
    glm::mat3 mdl_to_ndc = SEGfx->m_world_to_ndc_transform * mdl;
*/



    
    ////p0 = mdl_to_ndc * p0;
    //p0 = { -mdl_to_ndc[0].x, -mdl_to_ndc[1].y, mdl_to_ndc[2].z };
    //p1 = mdl_to_ndc * p1;
    //p2 = mdl_to_ndc * p2;
    //p3 = { -mdl_to_ndc[0].x, mdl_to_ndc[1].y, mdl_to_ndc[2].z };
    ////p3 = mdl_to_ndc * p3;

   
    glBindVertexArray(SEGfx->VAO);
    SEGfx->ShaderPgm_Texture.Use();

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Vertex, SEGfx->BindingPointA);
    glNamedBufferSubData(SEGfx->VBO, 0, sizeof(glm::vec3) * 4, points.data());

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Texture, SEGfx->BindingPointE);
    glNamedBufferSubData(SEGfx->VBO, sizeof(glm::vec3) * 4 + sizeof(glm::vec3) * 4, sizeof(glm::vec2) * 4, uv.data());
    assert(ID, "invalid texture being used!");
    glBindTextureUnit(0, ID);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, ID);
    GLuint loc = glGetUniformLocation(SEGfx->ShaderPgm_Texture.GetHandle(), "uTex2D");
    glUniform1i(loc, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glBindVertexArray(0);
    SEGfx->ShaderPgm_Texture.UnUse();
}

void SEDrawTexture(const SEFloat& x, const SEFloat& y, const SEFloat& scale_x, const SEFloat& scale_y, const SEFloat& angle, const std::vector<SEVec2>& uv, SEUint ID)
{
    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        glBindFramebuffer(GL_FRAMEBUFFER, SEGfx->frameBuffer);
    }
    glm::mat3 mat_scale = glm::mat3
    (
        glm::vec3(scale_x, 0, 0),
        glm::vec3(0, scale_y, 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_rot = glm::mat3
    (
        glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0),
        glm::vec3(-glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)), 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat3 mat_trans = glm::mat3
    (
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(x, y, 1)
    );

    glm::mat3 mdl = mat_trans * (mat_rot * mat_scale);
    glm::mat3 mdl_to_ndc = SEGfx->m_world_to_ndc_transform * mdl;

    glm::vec3 p0{ -1,-1,1 };
    glm::vec3 p1{ 1,-1,1 };
    glm::vec3 p2{ 1, 1,1 };
    glm::vec3 p3{ -1, 1,1 };
    p0 = mdl_to_ndc * p0;
    p1 = mdl_to_ndc * p1;
    p2 = mdl_to_ndc * p2;
    p3 = mdl_to_ndc * p3;

    std::vector<glm::vec3> points{ p0,p1,p2,p3 };
    glBindVertexArray(SEGfx->VAO);
    SEGfx->ShaderPgm_Texture.Use();

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Vertex, SEGfx->BindingPointA);
    glNamedBufferSubData(SEGfx->VBO, 0, sizeof(glm::vec3) * 4, points.data());

    glVertexArrayAttribBinding(SEGfx->VAO, SEGfx->Attribute_Texture, SEGfx->BindingPointE);
    glNamedBufferSubData(SEGfx->VBO, sizeof(glm::vec3) * 4 + sizeof(glm::vec3) * 4, sizeof(glm::vec2) * 4, uv.data());
    assert(ID, "invalid texture being used!");
    glBindTextureUnit(0, ID);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, ID);
    GLuint loc = glGetUniformLocation(SEGfx->ShaderPgm_Texture.GetHandle(), "uTex2D");
    glUniform1i(loc, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if (SEGfx && SEGfx->m_isEditor)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glBindVertexArray(0);
    SEGfx->ShaderPgm_Texture.UnUse();
}

bool SECheckWindowIsClose()
{
    if (glfwWindowShouldClose(SEGfx->m_window))
        return 0;
    return 1;
}

static double Time = 0.0;
static double count = 0.0;
double deltaTime = 0.0;
double elaspedTime = 0.0;
double lastTime = 0.0;
double maxPeriod = 0.0;

void SEFrameStart()
{
    if (SEGfx)
        maxPeriod = 1.0 / SEGfx->m_FPSLimit;
    Time = glfwGetTime();
    deltaTime = Time - lastTime;
    lastTime = Time;
    SEGfx->m_deltaTime = deltaTime;
  /*  if (deltaTime >= maxPeriod)
    {
        ++count;
    }*/
}

void SEFrameEnd()
{
    SEGfx->m_currentFPS = 1.0f / deltaTime;
    /*if (deltaTime >= maxPeriod)
    {
        elaspedTime = Time - lastTime;
        lastTime = Time;
        
        count = 0;
    }*/
}

double SEGetDeltaTime()
{
    if (SEGfx)
        return SEGfx->m_deltaTime;
    return 0;
}

double SEGetFPS() {
    if (SEGfx)
        return SEGfx->m_currentFPS;

    return 0;
}

void SESetDeltaTime(double dt)
{
    if (SEGfx)
        SEGfx->m_deltaTime = dt;
}

void SESetFullScreen(SEBool flag)
{
    if (SEIsFullScreen() == flag)
        return;

    if (flag)
    {
        // backup window position and window size
        glfwGetWindowPos(SEGfx->m_window, &SEGfx->winPos_x, &SEGfx->winPos_y);
        glfwGetWindowSize(SEGfx->m_window, &SEGfx->winSize_x, &SEGfx->winSize_y);

        // get resolution of monitor
        auto monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // switch to full screen
        SEGfx->m_window_height = mode->height;      // added on 20/2/23
        SEGfx->m_window_width = mode->width;
        glfwSetWindowMonitor(SEGfx->m_window, monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        // restore last window size and position
        SEGfx->m_window_height = SEGfx->winSize_y;// added on 20/2/23
        SEGfx->m_window_width = SEGfx->winSize_x;
        glfwSetWindowMonitor(SEGfx->m_window, nullptr, SEGfx->winPos_x, SEGfx->winPos_y, SEGfx->winSize_x, SEGfx->winSize_y, 0);
    }

}

bool SEIsFullScreen()
{
    return glfwGetWindowMonitor(SEGfx->m_window) != nullptr;
}

int SEGetWindowHeight()
{
    if (SEGfx) return SEGfx->m_window_height;
    return 0;
}

int SEGetWindowWidth()
{
    if (SEGfx) return SEGfx->m_window_width;
    return 0;
}

void SetCameraPosition(SEFloat x, SEFloat y)
{
    if (SEGfx)
    {
        SEGfx->Camera->m_center = glm::vec3(x, y, 1);
    }
}

void SESetCameraHeight(float height)
{
    if (SEGfx)
        SEGfx->Camera->m_height = height;
}

SEFloat GetCameraPositionX()
{
    if (SEGfx) return SEGfx->Camera->m_center.x;
    return 0;
}

SEFloat GetCameraPositionY()
{
    if (SEGfx) return SEGfx->Camera->m_center.y;
    return 0;
}

SEInt SEGetCameraHeight() {
    if (SEGfx)
        return (SEInt)SEGfx->Camera->m_height;
    return SEInt{};
}

SEInt SEGetCameraWidth()
{
    if (SEGfx)
        return (SEInt)SEGfx->Camera->m_width;
    return SEInt{};
}

glm::mat3 SEGetCameraMatrix()
{
    if (SEGfx)
        return SEGfx->Camera->SEGetCameraMatrix();
    return glm::mat3{};
}

glm::mat3 SEGetWorldToNDCMatrix()
{
    return SEGfx->m_world_to_ndc_transform;
}

glm::mat3 SEGetCameraProj() // testing
{
    return SEGfx->m_projection_transform;
}

SEUint SEGetFrameBuffer()
{
    if (SEGfx) return SEGfx->frameBuffer;
    return 0;
}

SEUint SEGetTextureBuffer()
{
    if (SEGfx) return SEGfx->frameBuffer_texture;
    return 0;
}

GLFWwindow* SEGetGLWindow()
{
    if (SEGfx)
        return SEGfx->m_window;
    return 0;
}

glm::vec3 SEGetCursor()
{
    return glm::vec3(SEGfx->mouse_x, SEGfx->mouse_y, 1);
}

glm::vec3 SEGetCursorInWorld()
{
    // convert mouse position in opengl viewport to NDC range
    auto mouse_x_ndc = SEGetCursor().x * 2 / SEGetWindowWidth() - 1.0f;
    auto mouse_y_ndc = 1.0f - SEGetCursor().y * 2 / SEGetWindowHeight();
    glm::vec3 mouse{ mouse_x_ndc, mouse_y_ndc, 1 };

    // convert ndc system to world system
    auto ndc_to_world_mat = glm::inverse(SEGetWorldToNDCMatrix());

    // get mouse position in world system
    return ndc_to_world_mat * mouse;
}

double SEGetCursorX()
{
    if (SEGfx)
        return SEGfx->mouse_x;
    return 0;
}

double SEGetCursorY()
{
    if (SEGfx)
        return SEGfx->mouse_y;
    return 0;
}

bool SECheckCursorDeadZone(SEVec2 CurrentPos, SEVec2 PreviousPos, int deadzone) {
    if (CurrentPos.x > (PreviousPos.x + deadzone) || CurrentPos.y > (PreviousPos.y + deadzone) || CurrentPos.x < (PreviousPos.x - deadzone) || CurrentPos.y < (PreviousPos.y - deadzone)) {
        return true;
    }

    return false;
}

SEBool SEIsMouseLeftPressed()
{
    if (SEGfx->m_MouseButton_Left_isClicked)
        return SEGfx->m_MouseButton_Left_isClicked;
    else return 0;
}

SEBool SEIsMouseRightPressed()
{
    if (SEGfx->m_MouseButton_Right_isClicked)
        return SEGfx->m_MouseButton_Right_isClicked;
    else return 0;
}

SEBool SEIsMouseLeftHeld()
{
    return SEGfx->m_MouseButton_Left_isHeld;
}

SEBool SEIsMouseRightHeld()
{
    return SEGfx->m_MouseButton_Right_isHeld;
}

SEFloat SEMouseScroll() {

    return SEGfx->m_MouseScroll_Offset_Y;

}

void ResetMouseScroll() {
    SEGfx->m_MouseScroll_Offset_Y = 0;
}

SEBool SEIsKeyTriggered(SEInt key)
{
    assert(key >= 0 && key <= 350, "out bounds");
    if (SEGfx)
    {
        if (SEGfx->KeyboardCharacters_State[key] == GLFW_PRESS && SEGfx->KeyboardCharacters_LastState[key] == GLFW_RELEASE) {
#if 0
            std::cout << key << " is triggered" << std::endl;
#endif
            return true;
        }
    }
    return false;
}

SEBool SEIsKeyHeldDown(SEInt key)
{
    assert(key >= 0 && key <= 350, "out bounds");
    if (SEGfx)
    {
        if (SEGfx->KeyboardCharacters_State[key] == GLFW_PRESS) {
#if 0
            std::cout << key << " is Held down" << std::endl;
#endif
            return true;
        }

        if (SEGfx->KeyboardCharacters_State[key] == GLFW_REPEAT && (SEGfx->KeyboardCharacters_LastState[key] == GLFW_REPEAT || SEGfx->KeyboardCharacters_LastState[key] == GLFW_PRESS)) {
            return true;
        }
    }
    return false;
}

SEBool SEIsKeyReleased(SEInt key)
{
    assert(key >= 0 && key <= 350, "out bounds");
    if (SEGfx)
    {
        if (SEGfx->KeyboardCharacters_State[key] == GLFW_RELEASE && (SEGfx->KeyboardCharacters_LastState[key] == GLFW_PRESS || SEGfx->KeyboardCharacters_LastState[key] == GLFW_REPEAT)) {
#if _DEBUG
            std::cout << key << " is released" << std::endl;
#endif
            return true;
        }
    }

    return false;
}

void SEUpdateKeyStates()
{
    SEGfx->KeyboardCharacters_LastState = SEGfx->KeyboardCharacters_State;
    SEGfx->m_MouseButton_Left_isClicked = 0;
    SEGfx->m_MouseButton_Right_isClicked = 0;
}

void SEKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)mods; (void)scancode; (void)window;
    if (SEGfx)
    {
        assert(key >= 0 && key <= 350);

        SEGfx->KeyboardCharacters_State[key] = action;
    }
}

void SEMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
    (void)mods; (void)window;
    if (SEGfx)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                SEGfx->m_MouseButton_Left_isClicked = 1;
                SEGfx->m_MouseButton_Left_isHeld = 1;
            }
            else if (action == GLFW_RELEASE)
                SEGfx->m_MouseButton_Left_isHeld = 0;
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            if (action == GLFW_PRESS)
            {
                SEGfx->m_MouseButton_Right_isClicked = 1;
                SEGfx->m_MouseButton_Right_isHeld = 1;
            }
            else if (action == GLFW_RELEASE)
                SEGfx->m_MouseButton_Right_isHeld = 0;
        }
    }
}

void SEFrameBufferCB(GLFWwindow* window, int width, int height)
{
    (void)window;
    std::cout << "fb called back " << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

SEVec2 SE_Bounded_Camera(SEVec2 _translate, SEVec2 _Cam_dimensions, SEVec4 _WorldBound)
{
    SEVec2
        topleft = { _translate.x - _Cam_dimensions.x, _translate.y + _Cam_dimensions.y },
        bottomleft = { _translate.x - _Cam_dimensions.x, _translate.y - _Cam_dimensions.y },
        bottomright = { _translate.x + _Cam_dimensions.x, _translate.y - _Cam_dimensions.y },
        topright = { _translate.x + _Cam_dimensions.x, _translate.y + _Cam_dimensions.y },
        final_translate = _translate;

    // x collisions(-ve x)
    if (topleft.x < _WorldBound.x || bottomleft.x < _WorldBound.x)
    {
        final_translate.x = _WorldBound.x + _Cam_dimensions.x;
    }

    // y collisions(-ve y)
    if (bottomleft.y < _WorldBound.y || bottomright.y < _WorldBound.y)
    {
        final_translate.y = _WorldBound.y + _Cam_dimensions.y;
    }

    // z collisions (+ve x)
    if (topright.x > _WorldBound.z || bottomright.x > _WorldBound.z)
    {
        final_translate.x = _WorldBound.z - _Cam_dimensions.x;
    }

    // w collisions (+ve y)
    if (topright.y > _WorldBound.w || topleft.y > _WorldBound.w)
    {
        final_translate.y = _WorldBound.w - _Cam_dimensions.y;
    }

    return final_translate;
}

void SESetWindowWidthHeight(int width, int height)
{
    SEGfx->m_window_width = width;
    SEGfx->m_window_height = height;

    glfwSetWindowSize(SEGetGLWindow(), SEGfx->m_window_width, SEGfx->m_window_height);
}
