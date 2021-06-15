#include "vk.h"

VkShaderModule _createShaderModule(vk::Device device,
                                   const std::vector<char>& code) {
  vk::ShaderModuleCreateInfo info;
  info.setCodeSize(code.size())
      .setPCode(reinterpret_cast<const uint32_t*>(code.data()));
  return device.createShaderModule(info);
}

#define DEFINE_VK_SHADER_CREATE(x)                                    \
  vk::PipelineShaderStageCreateInfo vk_shader_create_##x(             \
      vk_device* device, const std::vector<char>& code,               \
      const char* name = "main") {                                    \
    vk::Device _device = device->device;                              \
    VkShaderModule shaderModule = _createShaderModule(_device, code); \
    vk::PipelineShaderStageCreateInfo info;                           \
    info.setStage(vk::ShaderStageFlagBits::e##x)                      \
        .setModule(shaderModule)                                      \
        .setPName(name);                                              \
    return info;                                                      \
  }
// vertex & fragment shaders
DEFINE_VK_SHADER_CREATE(Vertex)
DEFINE_VK_SHADER_CREATE(Fragment)

vk_pipeline* vk_pipeline_create(vk_device* device) {
  auto ret = new vk_pipeline();
  ret->device = device;
  return ret;
}

void vk_pipeline_prepare_default(vk_pipeline* pipeline,
                                 VkExtent2D swapChainExtent) {
  {
    pipeline->vertexInput.setVertexBindingDescriptionCount(0)
        .setVertexAttributeDescriptionCount(0);
  }
  {
    pipeline->inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList)
        .setPrimitiveRestartEnable(vk::Bool32(false));
  }
  {
    vk::Viewport _viewport{};
    _viewport.x = 0.0f;
    _viewport.y = 0.0f;
    _viewport.width = (float)swapChainExtent.width;
    _viewport.height = (float)swapChainExtent.height;
    _viewport.minDepth = 0.0f;
    _viewport.maxDepth = 1.0f;
    vk::Rect2D _scissor{};
    _scissor.offset = vk::Offset2D{0, 0};
    _scissor.extent = swapChainExtent;
    pipeline->viewport.setViewportCount(1)
        .setViewports(_viewport)
        .setScissorCount(1)
        .setScissors(_scissor);
  }
  {
    pipeline->rasterizer.setDepthClampEnable(vk::Bool32(false))
        .setRasterizerDiscardEnable(vk::Bool32(false))
        .setPolygonMode(vk::PolygonMode::eFill)
        .setLineWidth(1.0f)
        .setCullMode(vk::CullModeFlagBits::eBack)
        .setFrontFace(vk::FrontFace::eClockwise)
        .setDepthBiasEnable(vk::Bool32(false));
  }
  {
    pipeline->multisampling.setSampleShadingEnable(vk::Bool32(false))
        .setRasterizationSamples(vk::SampleCountFlagBits::e1);
  }
  {
    vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;
    pipeline->colorBlend.setLogicOpEnable(vk::Bool32(false))
        .setLogicOp(vk::LogicOp::eCopy)
        .setAttachmentCount(1)
        .setAttachments(colorBlendAttachment)
        .setBlendConstants({0.0f, 0.0f, 0.0f, 0.0f});
  }
  {
    vk::Device device = pipeline->device->device;
    vk::PipelineLayoutCreateInfo info;
    info.setPushConstantRangeCount(0).setSetLayoutCount(0);
    pipeline->pipelineLayout = device.createPipelineLayout(info);
  }
}

void vk_pipeline_create_graphics_pipeline(vk_pipeline* pipeline) {
  vk::GraphicsPipelineCreateInfo pipelineInfo;
  pipelineInfo.setStageCount(pipeline->shaders.size())
      .setPStages(pipeline->shaders.data())
      .setPVertexInputState(&pipeline->vertexInput)
      .setPInputAssemblyState(&pipeline->inputAssembly)
      .setPViewportState(&pipeline->viewport)
      .setPRasterizationState(&pipeline->rasterizer)
      .setPMultisampleState(&pipeline->multisampling)
      .setPColorBlendState(&pipeline->colorBlend)
      .setLayout(pipeline->pipelineLayout)
      .setRenderPass(pipeline->renderPass)
      .setSubpass(pipeline->subpass)
      .setBasePipelineHandle(VK_NULL_HANDLE);
  vk::Device device = pipeline->device->device;
  auto result = device.createGraphicsPipeline(VK_NULL_HANDLE, pipelineInfo);
  pipeline->pipeline = result.value;
}
