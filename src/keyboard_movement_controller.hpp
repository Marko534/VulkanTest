#pragma once

#include "lve_game_object.hpp"
#include "lve_window.hpp"

namespace lve
{
  class KeyboardMovmentController
  {
  public:
    struct KeyMappings
    {
      int moveLeft = GLFW_KEY_A;
      int moveRight = GLFW_KEY_D;
      int moveForward = GLFW_KEY_W;
      int moveBackward = GLFW_KEY_S;
      int moveUp = GLFW_KEY_E;
      int moveDown = GLFW_KEY_Q;
      int lookLeft = GLFW_KEY_LEFT;
      int lookRight = GLFW_KEY_RIGHT;
      int lookUp = GLFW_KEY_UP;
      int lookDown = GLFW_KEY_DOWN;
      int ctrlFunction = GLFW_KEY_LEFT_CONTROL;
    };

    // KeyboardMovmentController() : firstMouse{true} {}

    void moveInPlaneXZ(GLFWwindow *window,
                       float dt,
                       LveGameObject &gameObject);

    void moveInOrbit(GLFWwindow *window,
                     float dt,
                     LveGameObject &gameObject);

    KeyMappings keys{};
    float moveSpeed{3.f};
    float lookSpeed{1.5f};

    // mouse look
    float mouseSensitivity{0.1f};
    float lastX;
    float lastY;
    bool firstMouse{true};

    // orbit
    glm::vec3 center{0.f};
    float orbitRadius{10};
    float orbitSpeed{3.f};
    float orbitSensitivity{0.75f};
  };
} // namespace lve
