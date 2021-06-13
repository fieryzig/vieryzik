#include "vk.h"

vk_instance* vk_instance_create(std::vector<const char*> extensions)
{
	auto ret = new vk_instance();
	vk::ApplicationInfo appInfo;
	appInfo
		.setPApplicationName("Vulkan Test")
		.setApplicationVersion(VK_MAKE_API_VERSION(0, 0, 0, 0))
		.setPEngineName("Vulkan Test")
		.setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
		.setApiVersion(VK_MAKE_API_VERSION(0, 1, 2, 162));
	vk::InstanceCreateInfo info;

	info.setFlags({})
		.setPNext(nullptr)
		.setPApplicationInfo(&appInfo)
		.setEnabledLayerCount(uint32_t(enabledLayers.size()))
		.setPpEnabledLayerNames(enabledLayers.data())
		.setEnabledExtensionCount(uint32_t(extensions.size()))
		.setPpEnabledExtensionNames(extensions.data());
	ret->instance = vk::createInstance(info);
	return ret;
}

void vk_instance_destroy(vk_instance* inst)
{
	inst->instance.destroy();
	delete(inst);
}
