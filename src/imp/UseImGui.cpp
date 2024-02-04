#include "head/UseImGui.hpp"
#include <iostream>

namespace lve
{

  void UseImGui::SetupVulkanWindow(ImGui_ImplVulkanH_Window *wd, LveDevice &device, int width, int height)
  {
    std::cout << device.surface() << "\n";
    wd->Surface = device.surface();

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(device.getPhysicalDevice(), wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    // Only works with v-sinc
    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};

    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(device.getPhysicalDevice(), wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

    // The 2 can be MAX_FRAMES_IN_FLIGHT
    ImGui_ImplVulkanH_CreateOrResizeWindow(device.getInstance(), device.getPhysicalDevice(), device.device(), wd, device.findPhysicalQueueFamilies().graphicsFamily, nullptr, width, height, 2);
  }

  void UseImGui::Init(GLFWwindow *window, LveDevice &device, LveRenderer &renderer)
  {
    VkResult err;
    // SetupVulkanWindow(wd, device, 1024, 1024);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device.getInstance();
    init_info.PhysicalDevice = device.getPhysicalDevice();
    init_info.Device = device.device();
    // this might be wrong
    init_info.QueueFamily = device.findPhysicalQueueFamilies().graphicsFamily;
    // it is one of these two
    init_info.Queue = device.graphicsQueue();
    // init_info.Queue = device.presentQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;

    VkDescriptorPoolSize pool_sizes[] =
        {
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1},
        };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1;
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    err = vkCreateDescriptorPool(device.device(), &pool_info, nullptr, &g_DescriptorPool);
    check_vk_result(err);

    init_info.DescriptorPool = g_DescriptorPool;
    init_info.Subpass = 0;
    // this should be 2 if not that MAX_FRAMES_IN_FLIGHT
    init_info.MinImageCount = 2;
    init_info.ImageCount = renderer.getSwapChainImageCount();
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    std::cout << "AAAAAAAAAAAAAAA\n";
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, renderer.getSwapChainRenderPass());
  }

  // void UseImGui::NewFrame()
  // {
  //   // feed inputs to dear imgui, start new frame
  //   ImGui_ImplOpenGL3_NewFrame();
  //   ImGui_ImplGlfw_NewFrame();
  //   ImGui::NewFrame();
  // }

  // // from https://github.com/conan-io/examples/blob/master/libraries/dear-imgui/basic/main.cpp
  // void render_conan_logo()
  // {
  //   ImDrawList *draw_list = ImGui::GetWindowDrawList();
  //   float sz = 300.0f;
  //   static ImVec4 col1 = ImVec4(68.0 / 255.0, 83.0 / 255.0, 89.0 / 255.0, 1.0f);
  //   static ImVec4 col2 = ImVec4(40.0 / 255.0, 60.0 / 255.0, 80.0 / 255.0, 1.0f);
  //   static ImVec4 col3 = ImVec4(50.0 / 255.0, 65.0 / 255.0, 82.0 / 255.0, 1.0f);
  //   static ImVec4 col4 = ImVec4(20.0 / 255.0, 40.0 / 255.0, 60.0 / 255.0, 1.0f);
  //   const ImVec2 p = ImGui::GetCursorScreenPos();
  //   float x = p.x + 4.0f, y = p.y + 4.0f;
  //   draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + sz, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y), ImColor(col1));
  //   draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + 0.5 * sz, y + 1.0 * sz), ImVec2(x, y + 0.75 * sz), ImColor(col2));
  //   draw_list->AddQuadFilled(ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + sz, y + 0.25 * sz), ImVec2(x + sz, y + 0.75 * sz), ImVec2(x + 0.5 * sz, y + 1.0 * sz), ImColor(col3));
  //   draw_list->AddLine(ImVec2(x + 0.75 * sz, y + 0.375 * sz), ImVec2(x + 0.75 * sz, y + 0.875 * sz), ImColor(col4));
  //   draw_list->AddBezierCurve(ImVec2(x + 0.72 * sz, y + 0.24 * sz), ImVec2(x + 0.68 * sz, y + 0.15 * sz), ImVec2(x + 0.48 * sz, y + 0.13 * sz), ImVec2(x + 0.39 * sz, y + 0.17 * sz), ImColor(col4), 10, 18);
  //   draw_list->AddBezierCurve(ImVec2(x + 0.39 * sz, y + 0.17 * sz), ImVec2(x + 0.2 * sz, y + 0.25 * sz), ImVec2(x + 0.3 * sz, y + 0.35 * sz), ImVec2(x + 0.49 * sz, y + 0.38 * sz), ImColor(col4), 10, 18);
  // }

  // void UseImGui::Update()
  // {
  //   ImGui::Begin("Conan Logo"); // Create a window called "Conan Logo" and append into it.
  //   render_conan_logo();        // draw conan logo if user didn't override update
  //   ImGui::End();
  // }

  // void UseImGui::Render()
  // {
  //   // Render dear imgui into screen
  //   ImGui::Render();
  //   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // }

  // void UseImGui::Shutdown()
  // {
  //   // Cleanup
  //   ImGui_ImplOpenGL3_Shutdown();
  //   ImGui_ImplGlfw_Shutdown();
  //   ImGui::DestroyContext();
  // }

  void UseImGui::check_vk_result(VkResult err)
  {
    if (err == 0)
      return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
      abort();
  }

}