#include "vk.h"

vk_instance* vk_instance_create()
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

	std::vector<const char*> instanceLayerNames, instanceExtensionNames;

	info.setFlags({})
		.setPNext(nullptr) //TODO
		.setPApplicationInfo(&appInfo)
		.setEnabledLayerCount(uint32_t(instanceLayerNames.size()))
		.setPpEnabledLayerNames(instanceLayerNames.data())
		.setEnabledExtensionCount(uint32_t(instanceExtensionNames.size()))
		.setPpEnabledExtensionNames(instanceExtensionNames.data());
	ret->vkInstance = vk::createInstance(info);
	return ret;
}

void vk_instance_destroy(vk_instance* inst)
{
	inst->vkInstance.destroy();
	delete(inst);
}

VkInstance_T * vk_instance_get_instance(vk_instance * inst)
{
	return inst->vkInstance;
}