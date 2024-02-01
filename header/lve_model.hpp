#pragma once

#include "lve_device.hpp"

// libs
#define GLM_FORCE_RADIANS
// May want to delete to make it simmilar to openGl
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>
#include <memory>
#include <iostream>

namespace lve
{
  class LveModel
  {
  public:
    struct Vertex
    {
      glm::vec3 position{};
      glm::vec3 color{};
      glm::vec3 normal{};
      glm::vec2 uv{};

      static std::vector<VkVertexInputBindingDescription> getBindingDescription();
      static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

      bool operator==(const Vertex &other) const
      {
        return position == other.position &&
               color == other.color &&
               normal == other.normal &&
               uv == other.uv;
      }
    };

    struct Builder
    {
      std::vector<Vertex> vertices{};
      std::vector<uint32_t> indices{};

      void loadModel(const std::string &filepath);
    };

    LveModel(LveDevice &device, const Builder &builder);
    ~LveModel();

    LveModel(const LveModel &) = delete;
    LveModel &operator=(const LveModel &) = delete;

    static std::unique_ptr<LveModel> createModelFromFile(LveDevice &device, const std::string filepath);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

  private:
    LveDevice &lveDevice;

    // vertex buffer
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;

    // index buffer
    bool hasIndexBuffer = false;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    uint32_t indexCount;

    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffers(const std::vector<uint32_t> &indices);
  };

} // namespace lve
