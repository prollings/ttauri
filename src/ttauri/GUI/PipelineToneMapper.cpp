// Copyright 2019 Pokitec
// All rights reserved.

#include "ttauri/GUI/PipelineToneMapper.hpp"
#include "ttauri/GUI/PipelineToneMapper_DeviceShared.hpp"
#include "ttauri/GUI/Window.hpp"
#include "ttauri/GUI/GUIDevice.hpp"

namespace tt::PipelineToneMapper {

using namespace tt;
using namespace std;

PipelineToneMapper::PipelineToneMapper(Window const &window) :
    Pipeline_vulkan(window)
{
}

void PipelineToneMapper::drawInCommandBuffer(vk::CommandBuffer commandBuffer)
{
    Pipeline_vulkan::drawInCommandBuffer(commandBuffer);


    device().toneMapperPipeline->drawInCommandBuffer(commandBuffer);

    commandBuffer.draw(
        3,
        1,
        0,
        0
    );
}

std::vector<vk::PipelineShaderStageCreateInfo> PipelineToneMapper::createShaderStages() const {
    return device().toneMapperPipeline->shaderStages;
}

std::vector<vk::DescriptorSetLayoutBinding> PipelineToneMapper::createDescriptorSetLayoutBindings() const {
    return {
        {
            0, // binding
            vk::DescriptorType::eInputAttachment,
            1, // descriptorCount
            vk::ShaderStageFlagBits::eFragment
        }
    };
}

vector<vk::WriteDescriptorSet> PipelineToneMapper::createWriteDescriptorSet() const
{
    return {
        {
            descriptorSet,
            0, // destBinding
            0, // arrayElement
            1, // descriptorCount
            vk::DescriptorType::eInputAttachment,
            &(window.colorDescriptorImageInfo),
            nullptr, // bufferInfo
            nullptr // texelBufferView
        } 
    };
}

ssize_t PipelineToneMapper::getDescriptorSetVersion() const
{
    return 1;
}

vk::PipelineDepthStencilStateCreateInfo PipelineToneMapper::getPipelineDepthStencilStateCreateInfo() const
{
    // No depth buffering in the Tone Mapper
    return {
        vk::PipelineDepthStencilStateCreateFlags(),
        VK_FALSE, // depthTestEnable;
        VK_FALSE, // depthWriteEnable;
        vk::CompareOp::eAlways, // depthCompareOp
        VK_FALSE, // depthBoundsTestEnable
        VK_FALSE, // stencilTestEnable,
        vk::StencilOpState(), // front
        vk::StencilOpState(), // back
        0.0f, // minDepthBounds
        1.0f, // maxDepthBounds
    };
}

}
