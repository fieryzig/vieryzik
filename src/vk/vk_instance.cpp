#include "vk.h"

#ifdef VDEBUK
PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance                                 instance,
                                                               const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo,
                                                               const VkAllocationCallbacks *              pAllocator,
                                                               VkDebugUtilsMessengerEXT *                 pMessenger )
{
  return pfnVkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance                    instance,
                                                            VkDebugUtilsMessengerEXT      messenger,
                                                            VkAllocationCallbacks const * pAllocator )
{
  return pfnVkDestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc( VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                 VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                                 void * /*pUserData*/ )
{
    std::ostringstream message;
    message << "VieryZik: "; 
    message << vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) << ": "
            << vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) << ":\n";
    message << "\t"
            << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
    message << "\t"
            << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
    message << "\t"
            << "message         = <" << pCallbackData->pMessage << ">\n";
    if ( 0 < pCallbackData->queueLabelCount ) {
        message << "\t" << "Queue Labels:\n";
        for ( uint8_t i = 0; i < pCallbackData->queueLabelCount; i++ ) {
            message << "\t\t"
                    << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
        }
    }
    if ( 0 < pCallbackData->cmdBufLabelCount ) {
        message << "\t"
                << "CommandBuffer Labels:\n";
        for ( uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ ) {
            message << "\t\t"
                    << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
        }
    }
    if ( 0 < pCallbackData->objectCount ) {
        message << "\t"
                << "Objects:\n";
        for ( uint8_t i = 0; i < pCallbackData->objectCount; i++ ) {
            message << "\t\t"
                    << "Object " << i << "\n";
            message << "\t\t\t"
                    << "objectType   = "
                    << vk::to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType ) ) << "\n";
            message << "\t\t\t"
                    << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
            if ( pCallbackData->pObjects[i].pObjectName ) {
                message << "\t\t\t"
                        << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
            }
        }
    }
    //std::cerr << message.str() << std::endl;
// #ifdef _WIN32
//     MessageBox( NULL, message.str().c_str(), "Alert", MB_OK );
// #else
//     std::cout << message.str() << std::endl;
// #endif
    
    return false;
}
#endif

vk_instance* vk_instance_create(std::vector<const char*> extensions)
{
#ifdef VDEBUK
    std::vector<vk::ExtensionProperties> props = vk::enumerateInstanceExtensionProperties();
    auto propertyIterator = std::find_if( props.begin(), props.end(), []( vk::ExtensionProperties const & ep ) {
      return strcmp( ep.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) == 0;
    } );
    if ( propertyIterator == props.end() ) {
        throw std::runtime_error("validation layers requested, but not available!");
    }
#endif
	auto ret = new vk_instance();
	vk::ApplicationInfo appInfo;
	appInfo
		.setPApplicationName("Vulkan Test")
		.setApplicationVersion(VK_MAKE_API_VERSION(0, 0, 0, 0))
		.setPEngineName("Vulkan Test")
		.setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
		.setApiVersion(VK_MAKE_API_VERSION(0, 1, 2, 162));
    ret->instance = vk::createInstance(
        vk::InstanceCreateInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            #ifdef VDEBUK
            enabledLayers,
            #else
            {},
            #endif
            extensions));
	// vk::InstanceCreateInfo info;
#ifdef VDEBUK
    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        ret->instance.getProcAddr( "vkCreateDebugUtilsMessengerEXT" ) );
    if ( !pfnVkCreateDebugUtilsMessengerEXT ) {
        throw std::runtime_error("GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function.");
    }
    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        ret->instance.getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ) );
    if ( !pfnVkDestroyDebugUtilsMessengerEXT ) {
        throw std::runtime_error("GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function.");
    }
    auto severityFlags(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    auto messageTypeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
    auto debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT(
        vk::DebugUtilsMessengerCreateFlagsEXT(),
        severityFlags,
        messageTypeFlags,
        debugMessageFunc,
        nullptr);
    ret->debugMessenger = ret->instance.createDebugUtilsMessengerEXT(
        vk::DebugUtilsMessengerCreateInfoEXT(
            {}, severityFlags, messageTypeFlags, &debugMessageFunc));
#endif
// 	info.setFlags({})
// 		.setPApplicationInfo(&appInfo)  
// #ifdef VDEBUK    
// 		.setEnabledLayerCount(uint32_t(enabledLayers.size()))
//         .setPpEnabledLayerNames(enabledLayers.data())
//         .setPNext(&debugCreateInfo)
// #else
//         .setEnabledLayerCount(0)
//         .setPNext(nullptr)
// #endif
//         .setEnabledExtensionCount(uint32_t(extensions.size()))
// 		.setPpEnabledExtensionNames(extensions.data())
// 	ret->instance = vk::createInstance(info);
	return ret;
}

void vk_instance_destroy(vk_instance* inst)
{
#ifdef VDEBUK
    inst->instance.destroyDebugUtilsMessengerEXT(inst->debugMessenger, nullptr);
    //pfnVkDestroyDebugUtilsMessengerEXT(static_cast<VkInstance>(inst->instance), inst->debugMessenger, nullptr);
#endif
	inst->instance.destroy();
	delete(inst);
}
