#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_vulkan.h"

// #include "../external/imgui/imgui_tables.cpp"
// #include "../external/imgui/imgui_internal.h"
// #include "../external/imgui/imgui_draw.cpp"
// #include "../external/imgui/imgui_widgets.cpp"
// #include "../external/imgui/imgui_demo.cpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve
{

  class LveWindow
  {
  public:
    LveWindow(int w, int h, std::string name);
    ~LveWindow();

    LveWindow(const LveWindow &) = delete;
    LveWindow &operator=(const LveWindow &) = delete;

    bool shouldClose() { return glfwWindowShouldClose(window); }

    VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    GLFWwindow *getGLFWwindow() const { return window; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    bool wasWindowResized() { return framebufferResized; }
    void resetWindowResizedFlag() { framebufferResized = false; }

  private:
    static void framebufferResizedCallback(GLFWwindow *window, int width, int height);
    void initWindow();

    int width;
    int height;
    bool framebufferResized = false;

    std::string windowName;
    GLFWwindow *window;

    void initImGui();
    void cleanupImGui();
  };

}