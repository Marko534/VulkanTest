#include "lve_pipeline.hpp"

// std
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve
{

    std::vector<char> LvePipeline::readFile(const std::string &filepath)
    {

        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void LvePipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath,  const PiplineConfigInfo &configInfo)
    {

        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }

    LvePipeline::LvePipeline(
        LveDevice &device,
        const std::string &vertFilepath,
        const std::string &fragFilepath,
        const PiplineConfigInfo &configInfo) : lveDevice{device}
    {
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }

    void LvePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS){
            throw std::runtime_error("Failed to create shader module.");
        }
    }

    PiplineConfigInfo LvePipeline::defaultPiplineConfigInfo(uint32_t width, uint32_t height){
        PiplineConfigInfo configInfo{};
        return configInfo;
    }

}