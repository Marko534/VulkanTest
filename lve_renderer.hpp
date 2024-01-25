#pragma once

#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

// sdt
#include <memory>
#include <vector>
#include <cassert>

namespace lve
{

  class LveRenderer
  {

  public:
    LveRenderer(LveWindow &window, LveDevice &device);
    ~LveRenderer();

    LveRenderer(const LveRenderer &) = delete;
    LveRenderer &operator=(const LveRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
      assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
      return commandBuffers[currentFrameIndex];
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    int getCurrentFrameIndex() const
    {
      assert(isFrameStarted && "Cannot get frameIndex when frame not in progress");
      return currentFrameIndex;
    }

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    LveWindow &lveWindow;
    LveDevice &lveDevice;
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currenImageIndex{0};
    int currentFrameIndex{0};
    bool isFrameStarted{false};
  };
}