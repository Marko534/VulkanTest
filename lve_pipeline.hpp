#pragma once

#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve
{

    struct PiplineConfigInfo
    {
    };
    class LvePipeline
    {
    public:
        LvePipeline(
            LveDevice &device,
            const std::string &vertFilepath,
            const std::string &fragFilepath,
            const PiplineConfigInfo &configInfo);

        ~LvePipeline() {};
        LvePipeline(const LvePipeline&) = delete;
        void operator=(const LvePipeline&) = delete;

        static PiplineConfigInfo defaultPiplineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filepath);

        void createGraphicsPipeline(
            const std::string &vertFilepath,
            const std::string &fragFilepath,
            const PiplineConfigInfo &configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        LveDevice& lveDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule verShaderModule;
        VkShaderModule fragShaderModule;
    };
}