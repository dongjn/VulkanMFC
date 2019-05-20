#include"vulkan_context.h"
#include<vulkan/vulkan.h>
#include<vulkan/vulkan_win32.h>
#include<vector>
#include<stdint.h>
#include<Windows.h>
#include<gpu/GrContext.h>
#include<gpu/vk/GrVkVulkan.h>
#include<gpu/vk/GrVkBackendContext.h>
#include<gpu/vk/GrVkExtensions.h>
#include<cassert>
#include<debugapi.h>
#include<varargs.h>
#include"utility.h"
using std::vector;
namespace seraphim {

#define AssertVulkan(msg) assert(vkResult== VK_SUCCESS && msg)
	VkResult vkResult;




	std::shared_ptr<VulkanContext> VulkanContext::gpVKContext{ nullptr };
	void *vulkanlib{ nullptr };
	static uint32_t UINT32MAX = static_cast<uint32_t>(-1);


	static VkBool32 __stdcall debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
		//std::cerr << "validation layer: " << msg << std::endl;
		slog(INFO_LEVEL, "LayerReport", "msg={", msg, "}");
		return VK_FALSE;
	}

	//auto loadFunction = [](const char* name, VkInstance instn, VkDevice device) ->PFN_vkVoidFunction {
	//	return PFN_vkVoidFunction(nullptr);
	//};



	PFN_vkVoidFunction LoadFunction(const char* name, VkInstance instn, VkDevice device) {
		if (device) {
			return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceProcAddr(device, name));
		}
		if (instn) {
			return reinterpret_cast<PFN_vkVoidFunction>(vkGetInstanceProcAddr(instn, name));
		}
		return PFN_vkVoidFunction(nullptr);
	}
	/**
	 *
	 * @param width
	 * @param height
	 * @param buffer
	 */
	HMODULE hModule;
	void VulkanContext::initGrContext() {
		//		hModule = LoadLibrary(nullptr);
		//		auto loadFunction = [this](const char* name, VkInstance instnance, VkDevice device)->PFN_vkVoidFunction {
		//			PFN_vkVoidFunction rst;
		//
		//			//rst = reinterpret_cast<PFN_vkVoidFunction>(GetProcAddress(hInstance,name));
		//			rst = reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceProcAddr(vkDevice, name));
		//			if (rst == nullptr) {
		//				rst = reinterpret_cast<PFN_vkVoidFunction>(vkGetInstanceProcAddr(iHandle.vkInstance, name));
		//			}
		//			return rst;
		//
		//
		//		};
		//        GrVkExtensions *vkExtension = new GrVkExtensions;
		//		vkExtension->init( loadFunction, iHandle.vkInstance, vkPhysicalDevice,
		//                          vkInstanceExtensionsName.size(), vkInstanceExtensionsName.data(),
		//                          vkDeviceExtensionName.size(), vkDeviceExtensionName.data());
		//
		//        GrVkBackendContext a;
		//        a.fInstance = iHandle.vkInstance;
		//        a.fPhysicalDevice = vkPhysicalDevice;
		//        a.fDevice = vkDevice;
		//        a.fQueue = graphicQueue;
		//        a.fGraphicsQueueIndex = graphicFamily;
		//        a.fMaxAPIVersion = VK_MAKE_VERSION(1, 0, 0);
		//        a.fVkExtensions = vkExtension;
		//        a.fGetProc = loadFunction;
		//        a.fInstanceVersion = VK_MAKE_VERSION(1, 0, 0);
		//
		//
		//        a.fFeatures = 0;
		//        a.fDeviceFeatures = &features;
		//        grContext = GrContext::MakeVulkan(a);
		//        assert(grContext.get());
		//
		//        VkPhysicalDeviceMemoryProperties memoryProperties;
		//        uint32_t imageBufferIndex = (std::numeric_limits<uint32_t>::max)();
		//        uint32_t mappingBufferIndex = (std::numeric_limits<uint32_t>::max)();
		//        VkDeviceSize imageBufferSize = 0;
		//        VkDeviceSize mappinngBufferSize = 0;
		//
		//        vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);
		//		VkFlags requImageMemoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		//		VkFlags requMappingType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		//        constexpr uint32_t graudUint32 = (std::numeric_limits<uint32_t>::max)();
		//        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		//            if ((imageBufferIndex == graudUint32) &&
		//                ((memoryProperties.memoryTypes[i].propertyFlags & requImageMemoryType) == requImageMemoryType)){
		//				imageBufferIndex = memoryProperties.memoryTypes[i].heapIndex;
		//                imageBufferSize = memoryProperties.memoryHeaps[i].size;
		//            }
		//            if ((mappingBufferIndex == graudUint32) &&
		//                ((memoryProperties.memoryTypes[i].propertyFlags & requMappingType) == requMappingType)) {
		//                mappingBufferIndex = memoryProperties.memoryTypes[i].heapIndex;
		//                mappinngBufferSize = memoryProperties.memoryHeaps[i].size;
		//            }
		//            if (imageBufferIndex < graudUint32 && mappingBufferIndex < graudUint32) {
		//                break;
		//            }
		//        }
		//        assert(mappingBufferIndex < graudUint32);
		//        assert(imageBufferIndex < graudUint32);
		//        //auto mathContxt = MathContext::get();
		//		uint32_t screenWidth = 0;
		//		uint32_t screenHeight = 0;  
		//        VkImage image;
		//        VkImageCreateInfo imageCreateInfo;
		//        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		//        imageCreateInfo.pNext = nullptr;
		//        imageCreateInfo.flags = 0;
		//        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		//        imageCreateInfo.format = VK_FORMAT_D24_UNORM_S8_UINT;
		//        imageCreateInfo.extent.width = wWidth;
		//        imageCreateInfo.extent.height = wHeight;
		//        imageCreateInfo.extent.depth = 1;
		//        imageCreateInfo.mipLevels = 1;
		//        imageCreateInfo.arrayLayers = 1;
		//        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		//        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		//        imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		//        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//        imageCreateInfo.queueFamilyIndexCount = 0;
		//        imageCreateInfo.pQueueFamilyIndices = nullptr;//&a.fGraphicsQueueIndex;
		//        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		//        VkResult vkResult = vkCreateImage(vkDevice, &imageCreateInfo, nullptr, &image);
		//        assert(vkResult == VK_SUCCESS);
		//        VkMemoryRequirements imageMemoryRequiremennts;
		//        vkGetImageMemoryRequirements(vkDevice, image, &imageMemoryRequiremennts);
		//        skiaImageMemorySize = imageMemoryRequiremennts.size;
		//        skiaImageMemoryOffset = 0;
		//
		//
		//
		//
		//
		//        VkMemoryAllocateInfo allocateInfo;
		//        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//        allocateInfo.pNext = nullptr;
		//        allocateInfo.allocationSize = imageMemoryRequiremennts.size;//.size * 4;
		//        allocateInfo.memoryTypeIndex = imageBufferIndex;
		//        vkResult = vkAllocateMemory(vkDevice, &allocateInfo, nullptr, &skiaImageMemory);
		//        assert(vkResult == VK_SUCCESS);
		//
		//
		//
		//
		//        VkMemoryAllocateInfo copyAllocateInfo;
		//        copyAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//        copyAllocateInfo.pNext = nullptr;
		//        copyAllocateInfo.allocationSize = imageMemoryRequiremennts.size;//.size * 4;
		//        copyAllocateInfo.memoryTypeIndex = mappingBufferIndex;
		//        vkResult = vkAllocateMemory(vkDevice, &copyAllocateInfo, nullptr, &copyMemory);
		//        assert(vkResult == VK_SUCCESS);
		//
		//        VkBufferCreateInfo copyBufferCreateInfo;
		//        copyBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//        copyBufferCreateInfo.pNext = nullptr;
		//        copyBufferCreateInfo.size = imageMemoryRequiremennts.size;
		//        copyBufferCreateInfo.pQueueFamilyIndices = &transferFamily;
		//        copyBufferCreateInfo.queueFamilyIndexCount = 1;
		//        copyBufferCreateInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;
		//        copyBufferCreateInfo.usage  = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		//        vkCreateBuffer(vkDevice,&copyBufferCreateInfo,nullptr,&copyBuffer);
		//        vkResult = vkBindBufferMemory(vkDevice,copyBuffer,copyMemory,0);
		//        uint8_t* pCopyBuf = nullptr;
		//        vkResult = vkMapMemory(vkDevice,copyMemory,0,imageMemoryRequiremennts.size,0, reinterpret_cast<void**>(&pCopyBuf));
		//		assert(vkResult == VK_SUCCESS);
		//        uint32_t* piCopyBuf = reinterpret_cast<uint32_t*>(pCopyBuf);
		//        for(int i = 0 ; i<1024*256;i++){
		//            piCopyBuf[i] = 0xff0000ff;
		//        }
		//
		//        VkMappedMemoryRange range;
		//        range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		//        range.size = 1024*1024*4;
		//        range.offset = 1024*1024*4;
		//        range.memory = copyMemory;
		//        vkResult =  vkFlushMappedMemoryRanges(vkDevice,1,&range);
		//        vkUnmapMemory(vkDevice,copyMemory);
		//        assert(vkResult == VK_SUCCESS);
		//
		//        VkMemoryAllocateInfo allocateMappingInfo;
		//        allocateMappingInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//        allocateMappingInfo.pNext = nullptr;
		//        allocateMappingInfo.allocationSize = imageMemoryRequiremennts.size;//.size * 4;
		//        allocateMappingInfo.memoryTypeIndex = mappingBufferIndex;
		//        vkResult = vkAllocateMemory(vkDevice, &allocateMappingInfo, nullptr, &localMemory);
		//        assert(vkResult == VK_SUCCESS);
		//
		//        VkBufferCreateInfo localMemoryCreateInfo;
		//        localMemoryCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//        localMemoryCreateInfo.pNext = nullptr;
		//        localMemoryCreateInfo.size = imageMemoryRequiremennts.size;
		//        localMemoryCreateInfo.pQueueFamilyIndices = &transferFamily;
		//        localMemoryCreateInfo.queueFamilyIndexCount = 1;
		//        localMemoryCreateInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;
		//        localMemoryCreateInfo.usage  = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		//        vkCreateBuffer(vkDevice,&localMemoryCreateInfo,nullptr,&localBuffer);
		//        vkResult = vkBindBufferMemory(vkDevice,localBuffer,localMemory,0);
		//        assert(vkResult == VK_SUCCESS);
		//
		//        uint8_t* pTempBuf= nullptr;
		//        vkMapMemory(vkDevice,localMemory,0,1024*1024*4,0, reinterpret_cast<void**>(&pTempBuf));
		////        memset(pTempBuf,0x55,1024*1024*4);
		//        uint32_t* p = reinterpret_cast<uint32_t*>(pTempBuf);
		//        for(int i = 0 ;i<1024*1024;i++){
		//            p[i] = 0xff00aabb;
		//        }
		//
		//        vkUnmapMemory(vkDevice,localMemory);
		//
		//
		//        vkDestroyImage(vkDevice, image, nullptr);
		//
		//
		//        //if(window){
		//
				//}
	}

	/**
	 *
	 * @param width
	 * @param height
	 * @param buffer
	 * @return
	 */
	static bool one = true;

	/****
	 *
	 * @param width
	 * @param height
	 * @param buffer
	 * @return
	 */
	SkCanvas *VulkanContext::makeBackend(uint32_t width, uint32_t height, uint8_t *buffer) {
		//
		//        do{
		//            if(backend.get()!= nullptr){
		//                if(backend->width == width  && backend->height == height && backend->dstBuffer == buffer){
		//                    break;
		//                }else{
		//                    vkDestroyImage(vkDevice,backend->vkImage,nullptr);
		//                }
		//            }
		//            one = false;
		//            VkResult vkResult;
		//            VkImage image;
		//            VkImageCreateInfo imageCreateInfo;
		//            imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		//            imageCreateInfo.pNext = nullptr;
		//            imageCreateInfo.flags = 0;
		//            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		//            imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;//VK_FORMAT_D24_UNORM_S8_UINT;
		//            imageCreateInfo.extent.width = width;
		//            imageCreateInfo.extent.height = height;
		//            imageCreateInfo.extent.depth = 1;
		//            imageCreateInfo.mipLevels = 1;
		//            imageCreateInfo.arrayLayers = 1;
		//            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		//            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		//            imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		//            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//            imageCreateInfo.queueFamilyIndexCount = 0;
		//            imageCreateInfo.pQueueFamilyIndices = nullptr;//&a.fGraphicsQueueIndex;
		//            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		//            vkResult = vkCreateImage(vkDevice, &imageCreateInfo, nullptr, &image);
		//            assert(vkResult == VK_SUCCESS);
		//            vkResult = vkBindImageMemory(vkDevice, image, skiaImageMemory, 0);
		//            VkMappedMemoryRange range;
		//            range.offset = 0;
		//            range.memory = localMemory;
		//            range.sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		//            range.pNext = nullptr;
		//            range.size = 1024*1024*4;
		//            vkFlushMappedMemoryRanges(vkDevice,1,&range);
		////            vkInvalidateMappedMemoryRanges()
		//            assert(VK_SUCCESS == vkResult);
		//
		//
		//            GrVkAlloc grVkAlloc(skiaImageMemory, 0, skiaImageMemorySize, GrVkAlloc::kNoncoherent_Flag);
		//
		//            GrVkImageInfo grImageInfo(image, grVkAlloc, VK_IMAGE_TILING_LINEAR,
		//                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		//                                      VK_FORMAT_R8G8B8A8_SRGB, 1);
		//            GrBackendTexture grBackedTexture(width, height, grImageInfo);
		//            sk_sp<SkColorSpace> colorSpace = SkColorSpace::MakeSRGB();
		//            SkSurfaceProps *surfaceProps = nullptr;
		//            sk_sp<SkSurface> surface = SkSurface::MakeFromBackendTexture(grContext.get(),
		//                                                                         grBackedTexture,
		//                                                                         GrSurfaceOrigin::kTopLeft_GrSurfaceOrigin,
		//                                                                         1,
		//                                                                         SkColorType::kRGBA_8888_SkColorType,
		//                                                                         colorSpace,
		//                                                                         surfaceProps);
		//            assert(surface.get());
		//            this->backend = std::make_shared<VulkanSkiaBackend>(width, height, buffer, surface, image, 0,0);
		//        }while(false);
		//
		return backend->getCanvas();
	}


	/***
	 *
	 * @param w
	 * @param h
	 * @param b
	 * @param s
	 * @param i
	 * @param deviceSize
	 * @param deviceOffset
	 */
	VulkanSkiaBackend::VulkanSkiaBackend(uint32_t w, uint32_t h, uint8_t* b, sk_sp<SkSurface> s, VkImage i, VkDeviceSize deviceSize, VkDeviceSize deviceOffset) :width(w),
		height(h),
		surface(s),
		vkImage(i),
		dstBuffer(b),
		deviceMemorySize(deviceSize),
		deviceMemoryOffset(deviceOffset)
	{
		canvas = surface->getCanvas();
	}


	VulkanContext::VulkanContext(HWND w, HINSTANCE h, UINT32 width, UINT32 height) :window(w), hInstance(h), wWidth(width), wHeight(height) {
		if (window) {

			iHandle.vkInstanceExtensionsName.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			iHandle.vkInstanceExtensionsName.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
			iHandle.vkInstanceExtensionsName.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

			dh.vkDeviceExtensionName.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		}
	}



	void VulkanContext::initVulkan() {

		//init physical_device
		initInstance();
		initDevice();
		initPresent();
		VkFenceCreateInfo fenceCreateInfo;
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = 0;
		vkCreateFence(dh.vkDevice, &fenceCreateInfo, nullptr, &dh.submitFence);
		AssertVulkan("CreateFence");



	}



	void VulkanContext::initInstance()
	{
		vulkan_api = UINT32MAX;
		VkApplicationInfo info;
		info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		info.apiVersion = VK_MAKE_VERSION(1, 0, 0);
		info.applicationVersion = 1;;
		info.engineVersion = 1;
		info.pApplicationName = "MathCore";
		info.pEngineName = "MathCore";
		info.pNext = nullptr;
		uint32_t count_extension = 0;
		uint32_t count_layer = 0;
		vkEnumerateInstanceLayerProperties(&count_layer, nullptr);
		vector<VkLayerProperties> layers(count_layer);
		iHandle.vkLayerExtensionName.clear();
		vkEnumerateInstanceLayerProperties(&count_layer, layers.data());
		for (auto layer : layers) {
			slog(INFO_LEVEL, TAG, layer.description);
			iHandle.vkLayerExtensionName.push_back(layer.layerName);
		}


		VkDebugReportCallbackCreateInfoEXT reportCallBack;
		reportCallBack.pfnCallback;

		vkEnumerateInstanceExtensionProperties(nullptr, &count_extension, nullptr);
		vector<VkExtensionProperties> instance_properties(count_extension);
		vkEnumerateInstanceExtensionProperties(nullptr, &count_extension,
			instance_properties.data());
		for (auto &p : instance_properties) {
			printf("MATH_VULKAN", "instance name=%s  version=%d", p.extensionName, p.specVersion);

		}
		VkInstanceCreateInfo instanceInfo;
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pNext = nullptr;
		instanceInfo.enabledExtensionCount = static_cast<uint32_t>(iHandle.vkInstanceExtensionsName.size());
		instanceInfo.ppEnabledExtensionNames = iHandle.vkInstanceExtensionsName.data();
		instanceInfo.pApplicationInfo = &info;
		instanceInfo.enabledLayerCount = iHandle.vkLayerExtensionName.size();
		instanceInfo.ppEnabledLayerNames = iHandle.vkLayerExtensionName.data();
		VkResult vkResult = vkCreateInstance(&instanceInfo, nullptr, &iHandle.vkInstance);
		assert(vkResult == VK_SUCCESS);
		//init layer debug
		VkDebugReportCallbackCreateInfoEXT callbackCreateInfo;
		callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		callbackCreateInfo.pNext = nullptr;
		callbackCreateInfo.pUserData = nullptr;
		callbackCreateInfo.pfnCallback = debugCallback;
		assert(vkResult == VK_SUCCESS);
		PFN_vkCreateDebugReportCallbackEXT createDebugReport = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(iHandle.vkInstance, "vkCreateDebugReportCallbackEXT");
		assert(createDebugReport);
		vkResult = createDebugReport(iHandle.vkInstance, &callbackCreateInfo, nullptr, &iHandle.debugReportCallbackExt);
	}

	void VulkanContext::initDevice()
	{
		uint32_t physical_count = 0;
		vkResult = vkEnumeratePhysicalDevices(iHandle.vkInstance, &physical_count, nullptr);
		assert(vkResult == VK_SUCCESS);
		vector<VkPhysicalDevice> physicalDevices(physical_count);
		vkResult = vkEnumeratePhysicalDevices(iHandle.vkInstance, &physical_count, physicalDevices.data());
		assert(vkResult == VK_SUCCESS);
		for (int i = 0; i < physical_count; i++) {
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
			slog(INFO_LEVEL, TAG, properties.deviceName);
		}
		dh.vkPhysicalDevice = physicalDevices[0];
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(dh.vkPhysicalDevice, &physicalDeviceProperties);
		vulkan_api = physicalDeviceProperties.apiVersion;

		uint32_t device_extension_count = 0;
		vkResult = vkEnumerateDeviceExtensionProperties(dh.vkPhysicalDevice, nullptr,
			&device_extension_count, nullptr);
		assert(vkResult == VK_SUCCESS);
		vector<VkExtensionProperties> device_extension(device_extension_count);
		vkResult = vkEnumerateDeviceExtensionProperties(dh.vkPhysicalDevice, nullptr,
			&device_extension_count,
			device_extension.data());
		assert(vkResult == VK_SUCCESS);

		for (int i = 0; i < device_extension.size(); i++) {
			slog(INFO_LEVEL, TAG, "DeviceExtension:", device_extension[i].extensionName);
		}

		vkGetPhysicalDeviceFeatures(dh.vkPhysicalDevice, &dh.features);
		uint32_t count_queue_family = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(dh.vkPhysicalDevice, &count_queue_family, nullptr);
		vector<VkQueueFamilyProperties> familyProperties(count_queue_family);
		vkGetPhysicalDeviceQueueFamilyProperties(dh.vkPhysicalDevice, &count_queue_family,
			familyProperties.data());
		for (uint32_t i = 0; i < familyProperties.size(); i++) {
			auto &properties = familyProperties[i];
			if (properties.queueCount > 0 && (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				dh.graphicFamily = i;
			}
			if (properties.queueCount > 0 && (properties.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
				dh.transferFamily = i;
			}

			if (properties.queueCount > 0 && (properties.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
				dh.calculateFamily = i;
			}
			if (dh.graphicFamily < (std::numeric_limits<uint32_t>::max)() && dh.transferFamily < (std::numeric_limits<uint32_t>::max)() && dh.calculateFamily < (std::numeric_limits<uint32_t>::max)()) {
				break;
			}
		}

		assert(dh.graphicFamily != UINT32MAX);
		assert(dh.calculateFamily != UINT32MAX);
		assert(dh.transferFamily != UINT32MAX);
		float queue_priorities[] = { 1.0f };

		vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		VkDeviceQueueCreateInfo grahicQueueCreateInfo;
		grahicQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		grahicQueueCreateInfo.pNext = nullptr;
		grahicQueueCreateInfo.queueFamilyIndex = dh.graphicFamily;
		grahicQueueCreateInfo.pQueuePriorities = &queue_priorities[0];
		grahicQueueCreateInfo.queueCount = 1;


		VkDeviceQueueCreateInfo transferQueueCreateInfo;
		transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transferQueueCreateInfo.pNext = nullptr;
		transferQueueCreateInfo.queueFamilyIndex = dh.graphicFamily;
		transferQueueCreateInfo.pQueuePriorities = &queue_priorities[0];
		transferQueueCreateInfo.queueCount = 1;
		queueCreateInfos.push_back(grahicQueueCreateInfo);
		queueCreateInfos.push_back(transferQueueCreateInfo);


		//CreateLogicDevice
		VkDeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext = nullptr;
		deviceCreateInfo.ppEnabledLayerNames = nullptr;// vkLayerExtensionName.data();
		deviceCreateInfo.enabledLayerCount = 0;// vkLayerExtensionName.size();
		deviceCreateInfo.pEnabledFeatures = nullptr;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(dh.vkDeviceExtensionName.size());
		deviceCreateInfo.ppEnabledExtensionNames = dh.vkDeviceExtensionName.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();


		vkResult = vkCreateDevice(dh.vkPhysicalDevice, &deviceCreateInfo, nullptr, &dh.vkDevice);
		assert(vkResult == VK_SUCCESS);
		vkGetDeviceQueue(dh.vkDevice, dh.graphicFamily, 0, &dh.graphicQueue);
		vkGetDeviceQueue(dh.vkDevice, dh.transferFamily, 0, &dh.transferQueue);

		VkCommandPoolCreateInfo poolCreateInfo;
		poolCreateInfo.queueFamilyIndex = dh.graphicFamily;
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolCreateInfo.pNext = nullptr;
		poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkCreateCommandPool(dh.vkDevice, &poolCreateInfo, nullptr, &dh.graphicPool);

		poolCreateInfo.queueFamilyIndex = dh.transferFamily;
		vkCreateCommandPool(dh.vkDevice, &poolCreateInfo, nullptr, &dh.transferPool);
		poolCreateInfo.queueFamilyIndex = dh.calculateFamily;
		vkCreateCommandPool(dh.vkDevice, &poolCreateInfo, nullptr, &dh.calculatePool);



	}

	//************************************
	// Method:    initPresent
	// FullName:  seraphim::VulkanContext::initPresent
	// Access:    private 
	// Returns:   void
	// Qualifier:
	//************************************
	void VulkanContext::initPresent()
	{

		//init surface 
		VkWin32SurfaceCreateInfoKHR createInfoKHR;
		//VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo;
		createInfoKHR.hwnd = window;
		//createInfoKHR.hinstance = 
		createInfoKHR.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
		createInfoKHR.pNext = nullptr;
		createInfoKHR.flags = 0;
		vkResult = vkCreateWin32SurfaceKHR(iHandle.vkInstance, &createInfoKHR, nullptr, &ph.surface);

		assert(vkResult == VK_SUCCESS);
		uint32_t miniImageCount = 0;
		VkSurfaceCapabilitiesKHR  surfaceCapabilities;
		//mini_image_count
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dh.vkPhysicalDevice, ph.surface, &surfaceCapabilities);
		miniImageCount = surfaceCapabilities.minImageCount + 1;
		if (miniImageCount > surfaceCapabilities.maxImageCount)
			miniImageCount = surfaceCapabilities.maxImageCount;
		//color_space
		//format 

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(dh.vkPhysicalDevice, ph.surface, &formatCount, nullptr);
		vector<VkSurfaceFormatKHR> formats(formatCount);
		if (formatCount != 0) {

			vkGetPhysicalDeviceSurfaceFormatsKHR(dh.vkPhysicalDevice, ph.surface, &formatCount, formats.data());
		}
		ph.colorSpace = formats[0].colorSpace;
		ph.format = formats[0].format;


		VkSwapchainCreateInfoKHR  swapchainCreateInfo;
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;//VK_STRUCTURE_TYPE_SWAPCHAIN_COUNTER_CREATE_INFO_EXT;
		swapchainCreateInfo.pNext = nullptr;
		swapchainCreateInfo.clipped = true;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.flags = 0;//
		swapchainCreateInfo.minImageCount = miniImageCount;
		swapchainCreateInfo.imageArrayLayers = miniImageCount;

		swapchainCreateInfo.imageExtent.width = surfaceCapabilities.currentExtent.width;
		swapchainCreateInfo.imageExtent.height = surfaceCapabilities.currentExtent.height;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageFormat = formats[0].format; //VK_FORMAT_R8G8B8A8_UNORM;
		swapchainCreateInfo.imageColorSpace = formats[1].colorSpace;//VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.surface = ph.surface;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = VK_NULL_HANDLE;// &graphicFamilyIndex;
		swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.oldSwapchain = ph.swapchain;
		vkResult = vkCreateSwapchainKHR(dh.vkDevice, &swapchainCreateInfo, nullptr, &ph.swapchain);
		VkSwapchainCreateInfoKHR swapChainInfo;
		assert(vkResult == VK_SUCCESS);
		uint32_t image_count;
		vkResult = vkGetSwapchainImagesKHR(dh.vkDevice, ph.swapchain, &image_count, nullptr);
		assert(vkResult == VK_SUCCESS);
		ph.images.resize(image_count);
		vkResult = vkGetSwapchainImagesKHR(dh.vkDevice, ph.swapchain, &image_count, ph.images.data());
		assert(vkResult == VK_SUCCESS);


		ph.imageCmdBuffer.resize(image_count);
		VkCommandBufferAllocateInfo allocInfo;
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.commandPool = dh.graphicPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = image_count;
		vkResult = vkAllocateCommandBuffers(dh.vkDevice, &allocInfo, ph.imageCmdBuffer.data());
		AssertVulkan("allocateCommandBuffers");

		VkSemaphoreCreateInfo semaphoreCreateInfo;
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		semaphoreCreateInfo.flags = 0;
		vkResult = vkCreateSemaphore(dh.vkDevice, &semaphoreCreateInfo, nullptr, &ph.acquireSemaphore);
		VkFenceCreateInfo fenceCreateInfo;
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = NULL;
		fenceCreateInfo.flags = 0;
		vkResult = vkCreateFence(dh.vkDevice, &fenceCreateInfo, nullptr, &ph.acquireFence);
		cleanBank();
	}

	void VulkanContext::cleanBank() {
		uint32_t image_count = ph.images.size();
		VkClearColorValue clear_color = {
				{ 1.0f, 0.8f, 0.4f, 0.0f }
		};

		VkImageSubresourceRange image_subresource_range = {
		  VK_IMAGE_ASPECT_COLOR_BIT,                    // VkImageAspectFlags                     aspectMask
		  0,                                            // uint32_t                               baseMipLevel
		  1,                                            // uint32_t                               levelCount
		  0,                                            // uint32_t                               baseArrayLayer
		  1                                             // uint32_t                               layerCount
		};

		for (uint32_t i = 0; i < image_count; ++i) {
			VkCommandBufferBeginInfo cmd_buffer_begin_info = {
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,  // VkStructureType                        sType
				nullptr,                                      // const void                            *pNext
				VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, // VkCommandBufferUsageFlags              flags
				nullptr                                       // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
			};


			VkImageMemoryBarrier barrier_from_present_to_clear = {
			  VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
			  nullptr,                                    // const void                            *pNext
			  VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          srcAccessMask
			  VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
			  VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                          oldLayout
			  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          newLayout
			  VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               srcQueueFamilyIndex
			  VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               dstQueueFamilyIndex
			  ph.images[i],                       // VkImage                                image
			  image_subresource_range                     // VkImageSubresourceRange                subresourceRange
			};

			VkImageMemoryBarrier barrier_from_clear_to_present = {
			  VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
			  nullptr,                                    // const void                            *pNext
			  VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          srcAccessMask
			  VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          dstAccessMask
			  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          oldLayout
			  VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                          newLayout
			  VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               srcQueueFamilyIndex
			  VK_QUEUE_FAMILY_IGNORED,                    // uint32_t                               dstQueueFamilyIndex
			  ph.images[i],                       // VkImage                                image
			  image_subresource_range                     // VkImageSubresourceRange                subresourceRange
			};

			vkBeginCommandBuffer(ph.imageCmdBuffer[i], &cmd_buffer_begin_info);
			vkCmdPipelineBarrier(ph.imageCmdBuffer[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_clear);

			vkCmdClearColorImage(ph.imageCmdBuffer[i], ph.images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &image_subresource_range);

			vkCmdPipelineBarrier(ph.imageCmdBuffer[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_clear_to_present);
			vkResult = vkEndCommandBuffer(ph.imageCmdBuffer[i]);
			AssertVulkan("CleanBack!");
		}


		VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		//VkSubmitInfo submit_info = {
		//  VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
		//  nullptr,                                      // const void                  *pNext
		//  1,                                            // uint32_t                     waitSemaphoreCount
		//  &ph.acquireSemaphore,              // const VkSemaphore           *pWaitSemaphores
		//  &wait_dst_stage_mask,                         // const VkPipelineStageFlags  *pWaitDstStageMask;
		//  1,                                            // uint32_t                     commandBufferCount
		//  &ph.imageCmdBuffer[image_index],  // const VkCommandBuffer       *pCommandBuffers
		//  1,                                            // uint32_t                     signalSemaphoreCount
		//  &Vulkan.RenderingFinishedSemaphore            // const VkSemaphore           *pSignalSemaphores
		//};

		//if (vkQueueSubmit(Vulkan.PresentQueue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
		//	return;
		//}

		//V
		//for (uint32_t i = 0; i < ph.images.size(); i++) {
		//VkCommandBufferBeginInfo beginInfo;
		//beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//beginInfo.pNext = nullptr;
		//beginInfo.pInheritanceInfo = nullptr;
		//vkBeginCommandBuffer(ph.imageCmdBuffer[i], &beginInfo);
		//VkClearColorValue clearVulae{ 0.4,0.8,0.6,0 };
		//vkCmdClearColorImage(ph.imageCmdBuffer[i], ph.images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearVulae, 0, nullptr);
		//vkResult = vkEndCommandBuffer(ph.imageCmdBuffer[i]);
		//AssertVulkan("vkCleanBank");
		//}
	}

	void VulkanContext::draw()
	{
		VkAcquireNextImageInfoKHR acquireInfo;
		VkResult presentResult;
		acquireInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;
		acquireInfo.pNext = nullptr;
		acquireInfo.semaphore = ph.acquireSemaphore;
		acquireInfo.fence = ph.acquireFence;
		acquireInfo.swapchain = ph.swapchain;
		acquireInfo.deviceMask = 0;
		uint32_t imageIndex;
		vkResult = vkAcquireNextImage2KHR(dh.vkDevice, &acquireInfo, &imageIndex);
		AssertVulkan("AcquireImage");
		VkPresentInfoKHR presentInfo;
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.pResults = &presentResult;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.swapchainCount = 1;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pSwapchains = &ph.swapchain;
		presentInfo.pWaitSemaphores = &ph.acquireSemaphore;
		vkResult = vkQueuePresentKHR(dh.graphicQueue, &presentInfo);
		//acquireInfo.
	}

	void VulkanContext::resize(UINT32 width, UINT32 height) {
		initPresent();

	}

	void VulkanContext::commit() {
		//        VkDeviceSize  imageBytes  = backend->width * backend->height *  4;
		//
		//        VkResult vkResult;
		//        vkResetFences(vkDevice,1,&submitFence);
		//
		//        VkCommandBufferBeginInfo beginInfo;
		//        beginInfo.pNext = nullptr;
		//        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		//        beginInfo.pInheritanceInfo = nullptr;
		//
		//        vkBeginCommandBuffer(transferCmdBuffer,&beginInfo);
		//        VkBufferImageCopy regions;
		//        regions.bufferOffset = 0;
		//        regions.bufferImageHeight = 0;
		//        regions.bufferRowLength = 0;
		//        regions.imageOffset.x = 0;
		//        regions.imageOffset.y = 0;
		//        regions.imageOffset.z = 0;
		//        regions.imageExtent.width = backend->width;
		//        regions.imageExtent.height = backend->height;
		//        regions.imageExtent.depth = 0;
		//        regions.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		//        regions.imageSubresource.baseArrayLayer = 0;
		//        regions.imageSubresource.mipLevel = 1;
		//        regions.imageSubresource.layerCount = 1;
		////        vkCmdCopyImageToBuffer
		//        vkCmdCopyImageToBuffer(transferCmdBuffer,backend->vkImage,VK_IMAGE_LAYOUT_UNDEFINED,copyBuffer,1,&regions);
		//
		//
		//        VkBufferCopy bufRegion;
		//        bufRegion.size = imageBytes - 1024*1024;//backend->width * backend->height* 4;
		//        bufRegion.srcOffset = 1024*1024;
		//        bufRegion.dstOffset = 1024*1024;
		//        vkCmdCopyBuffer(transferCmdBuffer,copyBuffer,localBuffer,1,&bufRegion);
		//        vkEndCommandBuffer(transferCmdBuffer);
		//        VkSubmitInfo submitInfo;
		//        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//        submitInfo.pNext =nullptr;
		//        submitInfo.commandBufferCount = 1;
		//        submitInfo.pCommandBuffers = &transferCmdBuffer;
		//        submitInfo.pSignalSemaphores = nullptr;
		//        submitInfo.pWaitDstStageMask = nullptr;
		//        submitInfo.waitSemaphoreCount  = 0 ;
		//        submitInfo.pSignalSemaphores = nullptr;
		//        submitInfo.signalSemaphoreCount = 0;
		//        uint8_t* test = backend->dstBuffer;
		//        printf("%p",test);
		//        vkResult  =  vkQueueSubmit(transferQueue,1,&submitInfo,submitFence);
		//        uint8_t* pBuf;
		//        vkResult = vkMapMemory(vkDevice,localMemory,0,imageBytes,0, reinterpret_cast<void**>(&pBuf));
		//        assert(VK_SUCCESS == vkResult);
		//        VkMappedMemoryRange memoryRange;
		//        memoryRange.size = imageBytes;
		//        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		//        memoryRange.memory = localMemory;
		//        memoryRange.offset = 0;
		//        vkResult = vkInvalidateMappedMemoryRanges(vkDevice,1,&memoryRange);
		//
		//        memcpy(backend->dstBuffer,pBuf,imageBytes);
		//
		//    auto l = VK_REMAINING_ARRAY_LAYERS;
		//        vkUnmapMemory(vkDevice,localMemory);
	}


	void VulkanContext::clean() {
		//VkResult vkResult;
		//uint32_t image_index;
		//VkSemaphore semahore;
		//VkSemaphoreCreateInfo semahoreCreateInfo;
		//semahoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		//semahoreCreateInfo.pNext = nullptr;
		//semahoreCreateInfo.flags = 0;
		//vkCreateSemaphore(vkDevice, &semahoreCreateInfo, nullptr, &semahore);
		//VkFence fence;
		//VkFenceCreateInfo fenceCreateInfo;
		//fenceCreateInfo.flags = 0;
		//fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		//fenceCreateInfo.pNext = nullptr;
		//vkCreateFence(vkDevice, &fenceCreateInfo, nullptr, &fence);




		//VkAcquireNextImageInfoKHR acquireInfo;
		//acquireInfo.deviceMask = 0;
		//acquireInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;
		//acquireInfo.pNext = nullptr;
		//acquireInfo.swapchain = swapchain;
		//acquireInfo.timeout = 1000;
		//acquireInfo.semaphore = semahore;
		//acquireInfo.fence = fence;
		//vkResult = vkAcquireNextImage2KHR(vkDevice, &acquireInfo, &image_index);
		//VkCommandBufferBeginInfo beginInfo;
		//beginInfo.pNext = nullptr;
		//beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		//beginInfo.pInheritanceInfo = nullptr;
		////for(uint32_t i = 0 ; )
		////vkBeginCommandBuffer(graphicCmdBuffer, &beginInfo);
		////



		////vkResult = vkEndCommandBuffer(graphicCmdBuffer);
		//VkSubmitInfo submitInfo;
		//submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//submitInfo.pNext = nullptr;
		//submitInfo.commandBufferCount = 1;
		//assert(false);
		////submitInfo.pCommandBuffers = &graphicCmdBuffer;
		//submitInfo.pSignalSemaphores = nullptr;
		//submitInfo.pWaitDstStageMask = nullptr;
		//submitInfo.waitSemaphoreCount = 0;
		//submitInfo.pSignalSemaphores = nullptr;
		//submitInfo.signalSemaphoreCount = 0;
		//vkResult = vkQueueSubmit(transferQueue, 1, &submitInfo, submitFence);
	}




	/**
	 *
	 */
	void VulkanContext::testRead() {
		//__android_log_print(ANDROID_LOG_INFO,"MathDuoTime","testRead--0-0");

		//sk_sp<SkColorSpace> colorSpace = SkColorSpace::MakeSRGB();
		//__android_log_print(ANDROID_LOG_INFO,"MathDuoTime","testRead--0-1");

		//auto dstImgInfo = SkImageInfo::Make(backend->width, backend->height,
		//                                    SkColorType::kRGBA_8888_SkColorType,
		//                                    SkAlphaType::kLastEnum_SkAlphaType, colorSpace);
		//__android_log_print(ANDROID_LOG_INFO,"MathDuoTime","testRead--0-2");

		//SkBitmap dstBitmap = SkBitmap();//(dstImgInfo);
		//__android_log_print(ANDROID_LOG_INFO,"MathDuoTime","testRead--0-3");

		//dstBitmap.setInfo(dstImgInfo);
		//__android_log_print(ANDROID_LOG_INFO,"MathDuoTime","testRead--0-4");

		//dstBitmap.setPixels(backend->dstBuffer);
		//__android_log_print(ANDROID_LOG_INFO,"MathDuoTime","testRead--0-5");

		//bool bRst = backend->surface->readPixels(dstBitmap, 0, 0);
		//__android_log_print(ANDROID_LOG_INFO,"MathDuoTime","testRead--0-6");

	}
	VulkanContext::~VulkanContext() {

	}
};
