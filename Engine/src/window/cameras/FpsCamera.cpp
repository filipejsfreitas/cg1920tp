#include "FpsCamera.h"

#include "../../glut.h"
#include "../Window.h"

#define _USE_MATH_DEFINES
#include <math.h>

using std::cout, std::endl;

namespace engine::window::cameras {
    FpsCamera::FpsCamera() {
        SphericalToCartesian();

        Window* window = Window::GetInstance();
        lastMouseX = window->GetWidth() / 2;
        lastMouseY = window->GetHeight() / 2;
    }

    FpsCamera::~FpsCamera() {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }

    void FpsCamera::PrintInfo() {
        cout << "Use WASD or the arrow keys to move the camera, and the mouse to change the yaw/pitch." << endl;
        cout << "Press Q to speed up the camera movement, and E to slow it down." << endl;
        cout << "NOTE: To toggle the mouse movement, press ESCAPE!" << endl;
    }

    void FpsCamera::UpdateCameraPosition() {
        gluLookAt(
            cameraPos.x, cameraPos.y, cameraPos.z,
            lookingAt.x, lookingAt.y, lookingAt.z,
            0, 1, 0
        );

        Window* window = Window::GetInstance();
        if(window->IsFocused() && escKeyPressed) {
            lastMouseX = window->GetWidth() / 2;
            lastMouseY = window->GetHeight() / 2;

            glutWarpPointer(lastMouseX, lastMouseY);
        }
    }

    void FpsCamera::HandleKeyPress(unsigned char key, int mouseX, int mouseY) {
        int deltaTime = Window::GetInstance()->GetDeltaTime();
        vec3 forward = ComputeForward();
        vec3 right = ComputeRight(forward);

        forward = scale(forward, speed/deltaTime);
        right = scale(right, speed/deltaTime);

        switch(key) {
            case 27:
                escKeyPressed = !escKeyPressed;

                if(escKeyPressed) {
                    glutSetCursor(GLUT_CURSOR_NONE);

                    Window* window = Window::GetInstance();
                    lastMouseX = window->GetWidth() / 2;
                    lastMouseY = window->GetHeight() / 2;

                    glutWarpPointer(lastMouseX, lastMouseY);
                } else {
                    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
                }

                break;
            case 'w':
            case 'W':
                cameraPos = {cameraPos.x + forward.x, cameraPos.y + forward.y, cameraPos.z + forward.z};
                break;
            case 's':
            case 'S':
                cameraPos = {cameraPos.x - forward.x, cameraPos.y - forward.y, cameraPos.z - forward.z};
                break;
            case 'a':
            case 'A':
                cameraPos = {cameraPos.x - right.x, cameraPos.y - right.y, cameraPos.z - right.z};
                break;
            case 'd':
            case 'D':
                cameraPos = {cameraPos.x + right.x, cameraPos.y + right.y, cameraPos.z + right.z};
                break;
            case 'q':
            case 'Q':
                speed += 5;
                break;
            case 'e':
            case 'E':
                speed -= 5;
                if(speed < 0)
                    speed = 0;
                break;
            default:
                break;
        }

        SphericalToCartesian();
    }

    void FpsCamera::HandleSpecialKeyPress(int key, int mouseX, int mouseY) {
        switch(key) {
            case GLUT_KEY_UP:
                this->HandleKeyPress('w', mouseX, mouseY);
                break;
            case GLUT_KEY_DOWN:
                this->HandleKeyPress('s', mouseX, mouseY);
                break;
            case GLUT_KEY_LEFT:
                this->HandleKeyPress('a', mouseX, mouseY);
                break;
            case GLUT_KEY_RIGHT:
                this->HandleKeyPress('d', mouseX, mouseY);
                break;
            default:
                break;
        }
    }

    void FpsCamera::HandlePassiveMouseMovement(int mouseX, int mouseY) {
        if(!escKeyPressed)
            return;

        int deltaX = lastMouseX - mouseX;
        int deltaY = lastMouseY - mouseY;

        yaw += 0.001 * deltaX;
        yaw = fmod(yaw, 2 * M_PI); // camAlpha %= 360

        pitch += 0.001 * deltaY;

        if(pitch < -M_PI_2) {
            pitch = -M_PI_2 + 0.0001;
        } else if(pitch > M_PI_2) {
            pitch = M_PI_2 - 0.0001;
        }

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        SphericalToCartesian();
    }

    void FpsCamera::SphericalToCartesian() {
        // LookingAt = ... + cameraPos
        // Isto mantém o vetor forward com tamanho de 1 unidade

        lookingAt.x = cos(pitch) * sin(yaw) + cameraPos.x;
        lookingAt.y = sin(pitch) + cameraPos.y;
        lookingAt.z = cos(pitch) * cos(yaw) + cameraPos.z;
    }

    vec3 FpsCamera::ComputeForward() {
        return normalize({lookingAt.x - cameraPos.x,
                          lookingAt.y - cameraPos.y,
                          lookingAt.z - cameraPos.z});
    }

    vec3 FpsCamera::ComputeRight(vec3 forward) {
        vec3 up = {0, 1, 0};

        return normalize(cross(forward, up));
    }

    vec3 FpsCamera::ComputeRight() {
        return this->ComputeRight(this->ComputeForward());
    }
}
