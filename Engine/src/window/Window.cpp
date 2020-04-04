#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::string, std::cerr, std::endl;
using engine::window::cameras::Camera;

using namespace gl;

namespace engine::window {
    Window::Window(const char* title, int width, int height):
    title(title), width(width), height(height)
    {}

    Window::~Window() {
        glfwSetWindowUserPointer(this->glfwWindow, nullptr);
        glfwDestroyWindow(this->glfwWindow);
        this->glfwWindow = nullptr;
    }

    bool Window::CreateWindow() {
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        
        this->glfwWindow = glfwCreateWindow(this->width, this->height, this->title.c_str(), nullptr, nullptr);

        if(!this->glfwWindow) {
            return false;
        }

        glfwSetWindowUserPointer(this->glfwWindow, this);

        glfwSetFramebufferSizeCallback(this->glfwWindow, callback_handlers::HandleFramebufferSizeChange);
        glfwSetKeyCallback(this->glfwWindow, callback_handlers::HandleKeyboardKeyPress);
        glfwSetMouseButtonCallback(this->glfwWindow, callback_handlers::HandleMouseKeyPress);
        glfwSetCursorPosCallback(this->glfwWindow, callback_handlers::HandleMouseMovement);

        glfwMakeContextCurrent(this->glfwWindow);
        glbinding::initialize(glfwGetProcAddress);
        glfwSwapInterval(1);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(Window::DebugCallback, 0);

        return true;
    }

    void Window::MainLoop() {
        if(this->glfwWindow == nullptr) {
            throw std::runtime_error("A janela ainda não foi criada!");
        }

    	if(this->scene == nullptr)
    	{
            throw std::runtime_error("Não foi definida uma cena a renderizar!");
    	}

    	if(this->camera == nullptr)
    	{
            throw std::runtime_error("Não foi definida uma câmara para navegar a cena!");
    	}

        glfwMakeContextCurrent(this->glfwWindow);

    	this->HandleFramebufferSizeChange(this->width, this->height);

        this->scene->InitGLSettings();

        this->PrintInfo();
        this->camera->PrintInfo();

        while(!glfwWindowShouldClose(this->glfwWindow)) {
            this->RenderFrame();
        }
    }

    void Window::RenderFrame() {
        this->ComputeDeltaTime();
        this->MeasureFps();

        this->scene->ClearPreviousFrame();

        glLoadIdentity();
        glMultMatrixd(&this->camera->viewMatrix[0][0]); // Set camera position in world

        this->scene->Render();

        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
    }

    void Window::PrintInfo() const {
        printf("Vendor: %s\n", glGetString(GL_VENDOR));
        printf("Renderer: %s\n", glGetString(GL_RENDERER));
        printf("Version: %s\n", glGetString(GL_VERSION));
    }

    void Window::ComputeDeltaTime() {
        static double lastTime = 0;
        double currentTime = glfwGetTime();

        this->deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }

    void Window::MeasureFps()
    {
        static double timebase = 0;
        static double time = 0;
        static int frame = 0;

        frame++;
        time = glfwGetTime();

        if(time - timebase > 1.0) {
            fps = (int) round(frame / (time - timebase));
            timebase = time;
            frame = 0;

            const string title = string(this->title) + " - FPS: " + std::to_string(fps);
            glfwSetWindowTitle(this->glfwWindow, title.c_str());
        }
    }

    void Window::HandleFramebufferSizeChange(int width, int height) {
        if(height == 0)
            height = 1;

        this->width = width;
        this->height = height;

        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        glViewport(0, 0, width, height);

        glm::dmat4 projectionMatrix = glm::perspective(glm::radians(45.0), double(width) / double(height), 1.0, 1000.0);
        // glm::dmat4 projectionMatrix = glm::ortho(-1, 1, -1, 1, 1, -1);
        glLoadMatrixd(&projectionMatrix[0][0]);

        glMatrixMode(GL_MODELVIEW);
    }

	void Window::DebugCallback(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const gl::GLchar* message, const void* userParam)
	{
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
	}
}
