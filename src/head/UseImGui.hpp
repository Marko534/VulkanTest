#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_vulkan.h"

#include "head/lve_device.hpp"
namespace lve
{
  class UseImGui
  {
  private:
    ImGui_ImplVulkanH_Window *wd;

    static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, LveDevice &device, int width, int height);
    static void check_vk_result(VkResult err);

  public:
    void Init(GLFWwindow *window, LveDevice &device);
    void NewFrame(){};
    virtual void Update(){};
    void Render(){};
    void Shutdown(){};
  };
}