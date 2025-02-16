#ifndef CG_TP_ENGINE_SRC_WINDOW_CAMERAS_FPSCAMERA_H_
#define CG_TP_ENGINE_SRC_WINDOW_CAMERAS_FPSCAMERA_H_

#include "Camera.h"

#include <glm/vec3.hpp>

namespace engine::window::cameras {
    /**
     * Implementação do tipo de câmara 3D em modo FPS.
     */
    class FpsCamera: public Camera {
    private:
        double speed = 40.0;

        double lastMouseX;
        double lastMouseY;

        double yaw = 3.14159;
        double pitch = -3.14159 / 4;

        glm::dvec3 lookingAt;
        glm::dvec3 cameraPos = glm::dvec3(0, 150, 150);

        glm::dvec3 ComputeForward();
        glm::dvec3 ComputeRight(glm::dvec3 forward);
        glm::dvec3 ComputeRight();

        void SphericalToCartesian();
    public:
        FpsCamera() = default;
        ~FpsCamera() override;

        void InitCamera(Window* window, GLFWwindow* glfwWindow) override;
    	
        void PrintInfo() const override;

        void UpdatePosition() override;

        void HandleKeyboardKeyPress(int key, int scanCode, int action, int mods) override;

        void HandleMouseKeyPress(int button, int action, int mods) override {}
        void HandleMouseMovement(double mouseX, double mouseY) override;

        void HandleScrollMovement(double xOffset, double yOffset) override;
    };
}

#endif //CG_TP_ENGINE_SRC_WINDOW_CAMERAS_FPSCAMERA_H_
