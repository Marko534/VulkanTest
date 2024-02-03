#pragma once

#include "head/lve_camera.hpp"
#include "head/lve_pipeline.hpp"
#include "head/lve_device.hpp"
#include "head/lve_game_object.hpp"

// std
#include <memory>
#include <vector>

namespace lve
{

  class SimpleRenderSystem
  {

  public:
    SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer,
                           std::vector<LveGameObject> &gameObjects,
                           const LveCamera &camera);

  private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    LveDevice &lveDevice;

    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
  };
}