#include "lve_model.hpp"

#include <cassert>
#include <cstring>

namespace lve
{
  LveModel::LveModel(LveDevice &device, const std::vector<Vertex> &vertices) : lveDevice{device}
  {
    createVertexBuffers(vertices);
  }
  LveModel::~LveModel()
  {
    vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
    vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
  }

  void LveModel::createVertexBuffers(const std::vector<Vertex> &vertices)
  {
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be atleast 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
    lveDevice.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        vertexBuffer,
        vertexBufferMemory);
    void *data;
    vkMapMemory(lveDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(lveDevice.device(), vertexBufferMemory);
  }

  void LveModel::draw(VkCommandBuffer commendBuffer)
  {
    vkCmdDraw(commendBuffer, vertexCount, 1, 0, 0);
  }

  void LveModel::bind(VkCommandBuffer commendBuffer)
  {
    VkBuffer buffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commendBuffer, 0, 1, buffers, offsets);
  }

  std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescription()
  {
    std::vector<VkVertexInputBindingDescription> bindingDescription(1);
    bindingDescription[0].binding = 0;
    bindingDescription[0].stride = sizeof(Vertex);
    bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  // Here is the big buffer
  std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions()
  {
    std::vector<VkVertexInputAttributeDescription> attributeDescription(2);
    attributeDescription[0].location = 0;
    attributeDescription[0].binding = 0;
    // size of each cell in the buffer
    attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription[0].offset = offsetof(Vertex, position);

    // Color
    attributeDescription[1].location = 1;
    attributeDescription[1].binding = 0;
    // size of each cell in the buffer
    attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescription[1].offset = offsetof(Vertex, color);

    return attributeDescription;
  }

} // namespace lve
