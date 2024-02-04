#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_vulkan.h"

class UseImGui
{
public:
  void Init(GLFWwindow *window, const char *glsl_version);
  void NewFrame();
  virtual void Update();
  void Render();
  void Shutdown();
};