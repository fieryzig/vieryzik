#include "vk.h"
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	auto window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	auto vk_inst = vk_instance_create();
	auto device = vk_device_create(vk_inst, nullptr, 0);
	auto pipeline = vk_pipeline_create(device);
	VkExtent2D extent{};
	vk_pipeline_prepare_default(pipeline, extent);
	vk_pipeline_create_graphics_pipeline(pipeline);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	vk_device_wait_idle(device);
	
	vk_device_destroy(device);
	vk_instance_destroy(vk_inst);
	return 0;
}