#pragma once

#include <vulkan/vulkan.hpp>

// vk_instance.cpp
struct vk_instance;
vk_instance* vk_instance_create();
void vk_instance_destroy(vk_instance*);
VkInstance_T* vk_instance_get_instance(vk_instance *);

// vk_device.cpp
struct vk_device;
vk_device* vk_device_create(vk_instance* vk_inst, const char** extension_names, uint32_t extension_names_count);
void vk_device_destroy(vk_device*);
VkDevice_T* vk_device_get_device(vk_device *);
void vk_device_wait_idle(vk_device*);

// vk_pipeline.cpp
#define DECLARE_VK_SHADER_CREATE( x ) \
vk::PipelineShaderStageCreateInfo vk_shader_create_##x(vk_device*, const std::vector<char>&, const char* name);
DECLARE_VK_SHADER_CREATE(Vertex)
DECLARE_VK_SHADER_CREATE(Fragment)
//vk_shader* vk_shader_create_Vertex(vk_device*, const std::vector<char>&, const char*);
//vk_shader* vk_shader_create_Fragment(vk_device*, const std::vector<char>&, const char*);

struct vk_pipeline;
vk_pipeline* vk_pipeline_create(vk_device* device);
void vk_pipeline_prepare_default(vk_pipeline * pipeline, VkExtent2D swapChainExtent);
void vk_pipeline_create_graphics_pipeline(vk_pipeline * pipeline);