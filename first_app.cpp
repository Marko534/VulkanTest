#include "first_app.hpp"

#include <stdexcept>
#include <array>
#include <cassert>

namespace lve
{
  FirstApp::FirstApp()
  {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
  }

  FirstApp::~FirstApp()
  {
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
  }

  void FirstApp::run()
  {

    while (!lveWindow.shouldClose())
    {
      glfwPollEvents();
      drawFrame();
    }

    vkDeviceWaitIdle(lveDevice.device());
  }

  void FirstApp::loadModels()
  {
    std::vector<LveModel::Vertex> vertices{
        {{0.0f, -0.866f}, {1.0f, 0.0f, 0.0f}}, // Top (Red)
        {{-0.866f, 0.5f}, {0.0f, 1.0f, 0.0f}}, // Bottom left (Green)
        {{0.866f, 0.5f}, {0.0f, 0.0f, 1.0f}}   // Bottom right (Blue)
    };
    lveModel = std::make_unique<LveModel>(lveDevice, vertices);
  }

  void FirstApp::createPipelineLayout()
  {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create pipeline layout!");
    }
  }

  void FirstApp::createPipeline()
  {
    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = lveSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<LvePipeline>(
        lveDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
  }

  void FirstApp::recreateSwapChain()
  {
    auto extent = lveWindow.getExtent();
    while (extent.width == 0 || extent.height == 0)
    {
      extent = lveWindow.getExtent();
      glfwWaitEvents();
    }

    vkDeviceWaitIdle(lveDevice.device());
    lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
    createPipeline();
  }

  void FirstApp::createCommandBuffers()
  {
    commandBuffers.resize(lveSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = lveDevice.getCommandPool();
    allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(lveDevice.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create commandBuffers!");
    }
  }

  void FirstApp::recordCommandBuffer(int imageIndex)
  {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
      throw std::runtime_error("Buffer failed to bigin recording!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = lveSwapChain->getRenderPass();
    renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, lveSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    lvePipeline->bind(commandBuffers[imageIndex]);
    lveModel->bind(commandBuffers[imageIndex]);
    lveModel->draw(commandBuffers[imageIndex]);

    lvePipeline->bind(commandBuffers[imageIndex]);
    lveModel->bind(commandBuffers[imageIndex]);
    lveModel->draw(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
    {
      throw std::runtime_error("Faled to record command buffer!");
    }
  }

  void FirstApp::drawFrame()
  {
    uint32_t imageIndex;
    auto result = lveSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
      recreateSwapChain();
      return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
      throw std::runtime_error("Faled to acquire swapChain image!");
    }

    recordCommandBuffer(imageIndex);
    result = lveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized())
    {
      lveWindow.resetWindowResizedFlag();
      recreateSwapChain();
      return;
    }
    if (result != VK_SUCCESS)
    {
      throw std::runtime_error("AAAAAAAAAAAAAAAAA!");
    }
  }

}