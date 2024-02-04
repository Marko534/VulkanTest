#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_vulkan.h"

#include "head/lve_device.hpp"
#include "head/lve_renderer.hpp"
namespace lve
{
  class UseImGui
  {
  private:
    VkDescriptorPool g_DescriptorPool{VK_NULL_HANDLE};

    static void SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, LveDevice &device, int width, int height);
    static void check_vk_result(VkResult err);

    // testing
    bool show_demo_window = {true};
    bool show_another_window{false};
    ImVec4 clear_color{ImVec4(0.45f, 0.55f, 0.60f, 1.00f)};

    ImGuiIO &io;

  public:
    UseImGui(GLFWwindow *window, LveDevice &device, LveRenderer &renderer);
    void Init(GLFWwindow *window, LveDevice &device, LveRenderer &renderer);
    void NewFrame();
    virtual void Update();
    void Render();
    void Shutdown(LveDevice &device);
  };
}