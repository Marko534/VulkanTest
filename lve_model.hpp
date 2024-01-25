#pragma once

#include "lve_device.hpp"

// libs
#define GLM_FORCE_RADIANS
// May want to delete to make it simmilar to openGl
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace lve
{
  class LveModel
  {
  public:
    struct Vertex
    {
      glm::vec3 position;
      glm::vec3 color;

      static std::vector<VkVertexInputBindingDescription> getBindingDescription();
      static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    LveModel(LveDevice &device, const std::vector<Vertex> &vertices);
    ~LveModel();

    LveModel(const LveModel &) = delete;
    LveModel &operator=(const LveModel &) = delete;

    void bind(VkCommandBuffer commendBuffer);
    void draw(VkCommandBuffer commendBuffer);

  private:
    LveDevice &lveDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;

    void createVertexBuffers(const std::vector<Vertex> &vertices);
  };

} // namespace lve
