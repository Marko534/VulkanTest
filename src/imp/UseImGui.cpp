#include "head/UseImGui.hpp"
#include <iostream>

namespace lve
{

  UseImGui::UseImGui(GLFWwindow *window, LveDevice &device, LveRenderer &renderer) : io(ImGui::GetIO())
  {
    // io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    Init(window, device, renderer);
  }
  void UseImGui::Init(GLFWwindow *window, LveDevice &device, LveRenderer &renderer)
  {
    VkResult err;
    // SetupVulkanWindow(wd, device, 1024, 1024);

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

  void UseImGui::NewFrame()
  {
    // feed inputs to dear imgui, start new frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void UseImGui::Update()
  {
    if (show_demo_window)
      ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.

    // 3. Show another simple window.
    if (show_another_window)
    {
      ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
        show_another_window = false;
      ImGui::End();
    }
  }

  void UseImGui::Render(VkCommandBuffer commandBuffer)
  {
    // Render dear imgui into screen
    ImGui::Render();
    ImDrawData *draw_data = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);

    // const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    // if (!is_minimized)
    // {
    //   wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
    //   wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
    //   wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
    //   wd->ClearValue.color.float32[3] = clear_color.w;
    //   FrameRender(wd, draw_data);
    //   FramePresent(wd);
    // }
  }

  void UseImGui::Shutdown(LveDevice &device)
  {
    // Cleanup
    vkDeviceWaitIdle(device.device());
    // check_vk_result(err);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void UseImGui::check_vk_result(VkResult err)
  {
    if (err == 0)
      return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
      abort();
  }

}