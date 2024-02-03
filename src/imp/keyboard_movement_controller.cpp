#include "head/keyboard_movement_controller.hpp"

#include <iostream>

namespace lve
{
  void KeyboardMovmentController::moveInPlaneXZ(GLFWwindow *window,
                                                float dt,
                                                LveGameObject &gameObject)
  {
    double xposd;
    double yposd;
    glfwGetCursorPos(window, &xposd, &yposd);

    // mouseLook
    float xpos = static_cast<float>(xposd);
    float ypos = static_cast<float>(yposd);
    if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    glm::vec3 rotate{0};

    rotate.y += xoffset;
    rotate.x += yoffset;

    // keybord look
    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
      rotate.y += 1.f;
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
      rotate.y -= 1.f;
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
      rotate.x += 1.f;
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
      rotate.x -= 1.f;
    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
    {
      gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
    }

    // limit pitch values between about +/- 85ish degrees
    gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
    gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

    float yaw = gameObject.transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
      moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
      moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
      moveDir += rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
      moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
      moveDir += upDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
      moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
    {
      gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
  }

  void KeyboardMovmentController::moveInOrbit(GLFWwindow *window, float dt, LveGameObject &gameObject)
  {
    orbitRadius = std::sqrt(gameObject.transform.translation.x * gameObject.transform.translation.x + gameObject.transform.translation.y * gameObject.transform.translation.y +
                            gameObject.transform.translation.z * gameObject.transform.translation.z);

    double xposd, yposd;
    glfwGetCursorPos(window, &xposd, &yposd);

    float xpos = static_cast<float>(xposd);
    float ypos = static_cast<float>(yposd);

    if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xoffset *= orbitSensitivity;
    yoffset *= orbitSensitivity;

    glm::vec3 rotate{0};
    rotate.y += xoffset;
    rotate.x += yoffset;

    // Apply rotation based on mouse input
    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon() &&
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
      gameObject.transform.rotation += lookSpeed * dt * rotate;
    }

    // limit pitch values between about +/- 85ish degrees
    gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
    gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

    float yaw = gameObject.transform.rotation.y;
    float pitch = gameObject.transform.rotation.x;

    // Handle keyboard input for moving forward and backward
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
    {
      orbitRadius -= moveSpeed * dt;
    }
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
    {
      orbitRadius += moveSpeed * dt;
    }

    // orbitRadius = glm::clamp(orbitRadius, minOrbitRadius, maxOrbitRadius);

    if ((glfwGetKey(window, keys.ctrlFunction) == GLFW_PRESS))
    {
      orbitRadius -= yoffset * dt * moveSpeed;
    }
    // Calculate new camera position in orbit around the center
    float orbitX = center.x + orbitRadius * sin(yaw) * cos(pitch);
    float orbitY = center.y + orbitRadius * sin(pitch);
    float orbitZ = center.z + orbitRadius * cos(yaw) * cos(pitch);

    // Update camera position
    gameObject.transform.translation = glm::vec3(orbitX, orbitY, orbitZ);
  }

} // namespace lve
