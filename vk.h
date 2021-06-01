#pragma once

#include <vulkan/vulkan.hpp>

// instance
struct vk_instance
{
	vk::Instance		vkInstance = nullptr;
};
vk_instance* vk_instance_create();
void vk_instance_destroy(vk_instance*);

// device
struct vk_device
{
	vk::Device			vkDevice = nullptr;
	vk::PhysicalDevice	vkPhysicalDevice = nullptr;
};
vk_device* vk_device_create(vk_instance* vk_inst, const char** extension_names, uint32_t extension_names_count);
void vk_device_destroy(vk_device*);
void vk_device_wait_idle(vk_device*);

// shaders
#define DECLARE_VK_SHADER_CREATE( x ) \
vk::PipelineShaderStageCreateInfo vk_shader_create_##x(vk_device*, const std::vector<char>&, const char* name);
DECLARE_VK_SHADER_CREATE(Vertex)
DECLARE_VK_SHADER_CREATE(Fragment)

// pipeline
struct vk_pipeline // only support graphics pipeline for now
{
	vk_device * device;
	vk::Pipeline pipeline = nullptr;
	std::vector<vk::PipelineShaderStageCreateInfo> shaders;
	vk::PipelineVertexInputStateCreateInfo vertexInput;
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	vk::PipelineViewportStateCreateInfo viewport;
	vk::PipelineRasterizationStateCreateInfo rasterizer;
	vk::PipelineMultisampleStateCreateInfo multisampling;
	vk::PipelineColorBlendStateCreateInfo colorBlend;
	vk::PipelineLayout pipelineLayout;
	vk::RenderPass renderPass;
	uint32_t subpass;
};
vk_pipeline* vk_pipeline_create(vk_device* device);
void vk_pipeline_prepare_default(vk_pipeline * pipeline, VkExtent2D swapChainExtent);
void vk_pipeline_create_graphics_pipeline(vk_pipeline * pipeline);

// surface
struct vk_surface
{
	vk::SurfaceKHR surface;
};

// swapchain
struct vk_swapchain
{
	vk::SwapchainKHR swapchain;
	std::vector<vk::Image> images;
	vk::Format imageFormat;
	vk::Extent2D extent;
	std::vector<vk::ImageView> imageViews;
	std::vector<vk::Framebuffer> framebuffers;
};
vk_swapchain* vk_swapchain_create(vk_device* device, vk_surface* surface);