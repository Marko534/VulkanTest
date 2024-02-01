#include "lve_renderer.hpp"

// std
#include <stdexcept>
#include <array>
#include <cassert>

// AAAAAAAA
#include <glm/gtc/constants.hpp>

namespace lve
{
  LveRenderer::LveRenderer(LveWindow &window, LveDevice &device) : lveWindow{window}, lveDevice{device}
  {
    recreateSwapChain();
    createCommandBuffers();
  }

  LveRenderer::~LveRenderer()
  {
    freeCommandBuffers();
  }

  void LveRenderer::recreateSwapChain()
  {
    auto extent = lveWindow.getExtent();
    while (extent.width == 0 || extent.height == 0)
    {
      extent = lveWindow.getExtent();
      glfwWaitEvents();
    }

    vkDeviceWaitIdle(lveDevice.device());

    if (lveSwapChain == nullptr)
    {
      lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
    }
    else
    {
      std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapChain);
      lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, oldSwapChain);

      if (!oldSwapChain->compareSwapFormats(*lveSwapChain.get()))
      {
        // you can make this better
        std::runtime_error("SwapChain image/deptb format has changed!");
      }
    }
    // we will be back
  }

  void LveRenderer::createCommandBuffers()
  {
    commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

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

  void LveRenderer::freeCommandBuffers()
  {
    vkFreeCommandBuffers(lveDevice.device(),
                         lveDevice.getCommandPool(),
                         static_cast<uint32_t>(commandBuffers.size()),
                         commandBuffers.data());
    commandBuffers.clear();
  }

  VkCommandBuffer LveRenderer::beginFrame()
  {
    assert(!isFrameStarted && "Can't call beginFrame while frame aready in progress");

    auto result = lveSwapChain->acquireNextImage(&currenImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
      recreateSwapChain();
      return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
      throw std::runtime_error("Faled to acquire swapChain image!");
    }

    isFrameStarted = true;
    auto commandBuffer = getCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
      throw std::runtime_error("Buffer failed to bigin recording!");
    }
    return commandBuffer;
  }
  void LveRenderer::endFrame()
  {
    assert(isFrameStarted && "Can't call endFrame while frame aready not in progress");
    auto commandBuffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
      throw std::runtime_error("Faled to record command buffer!");
    }

    auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currenImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized())
    {
      lveWindow.resetWindowResizedFlag();
      recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
      throw std::runtime_error("Faled to present swap chain image!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
  }

  void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
  {
    assert(isFrameStarted && "Can't call beginSwapChainRenderPass while frame aready not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin reder pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = lveSwapChain->getRenderPass();
    renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(currenImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 0.1f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, lveSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
  }
  void LveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
  {
    assert(isFrameStarted && "Can't call endSwapChainRenderPass while frame aready not in progress");
    assert(commandBuffer == getCurrentCommandBuffer() && "Can't end reder pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);
  }

}