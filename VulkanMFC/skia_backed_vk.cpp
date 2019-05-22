#include "skia_backed_vk.h"
#include<vulkan.h>
#include<set>
#include<assert.h>
namespace seraphim {
	struct StrCmp {
		 bool operator()(const char* l,const char* r){
			return strcmp(l, r) != 0;
		}
	};
	using std::set ;
	using std::vector;
	const vector<const char* > InstanceLayerFunctions{
		"vkEnumeratePhysicalDevices","vkGetPhysicalDeviceFormatProperties","vkGetPhysicalDeviceImageFormatProperties","vkGetPhysicalDeviceProperties"
		,"vkGetPhysicalDeviceQueueFamilyProperties","vkGetPhysicalDeviceMemoryProperties","vkEnumerateInstanceExtensionProperties","vkEnumerateDeviceExtensionProperties"
		,"vkEnumerateInstanceLayerProperties","vkEnumerateDeviceLayerProperties","vkEnumerateInstanceVersion","vkCreateInstance","vkGetPhysicalDeviceFeatures","vkGetPhysicalDeviceSparseImageFormatProperties"
		,"vkDestroyInstance","vkCreateDevice","vkDestroyDevice"
	};

#ifdef _WIN32
	static HMODULE ghModule = {NULL};
	static bool globalInit() {
		ghModule = LoadLibrary("VulkanMFC.exe");
		if (!ghModule)
			return false;
		//check 
		void* check = GetProcAddress(ghModule, "vkEnumerateDeviceExtensionProperties");
		if (!check)
			return false;
		return true;
	}
#endif

	shared_ptr<SkiaBackedVK> SkiaBackedVK::self{nullptr};
	shared_ptr<seraphim::SkiaBackedVK> SkiaBackedVK::get()
	{
		if(self)
			return self;
		auto vkCtx = VulkanContext::get();
		if (!vkCtx)
			return nullptr;
		auto s = new SkiaBackedVK(vkCtx);
		if (!s->init()) {
			delete s;
			return nullptr;
		}
		self.reset(s);
		return self;
		
	}

	shared_ptr<seraphim::SkiaBackedVK> SkiaBackedVK::make(shared_ptr<VulkanContext> vkContext)
	{
		if (self)
			return self;
		auto ctx = VulkanContext::make(NULL, NULL, 0, 0);
		auto s = new SkiaBackedVK(ctx);
		if (!s->init()) {
			delete s;
			return nullptr;
		}
		self.reset(s);
		return self;
	}


	bool SkiaBackedVK::init()
	{
		bool rst = false;
		do {
	
			auto load_function = [this](const char* name, VkInstance instance, VkDevice device)->PFN_vkVoidFunction {
				PFN_vkVoidFunction function{ nullptr };
				//d;
				auto itr = std::find_if(InstanceLayerFunctions.begin(), InstanceLayerFunctions.end(), [name](const char* l)->bool {
					return strcmp(name, l) == 0;
				});


				if (itr != InstanceLayerFunctions.end()) {
					function = vkGetInstanceProcAddr(vkContext->ih.vkInstance, name);
				}
				else {
					function = vkGetDeviceProcAddr(vkContext->dh.vkDevice, name);
				}
				assert(function!=nullptr);
				return (function);

			};
			GrVkExtensions vkExtension;//= GrVkExtensions;
								vkExtension.init(load_function, vkContext->ih.vkInstance, vkContext->dh.vkPhysicalDevice,
												  vkContext->ih.vkInstanceExtensionsName.size(),
												  vkContext->ih.vkInstanceExtensionsName.data(),
												  vkContext->dh.vkDeviceExtensionName.size(), vkContext->dh.vkDeviceExtensionName.data());
		//
		        GrVkBackendContext a;
		        a.fInstance = vkContext->ih.vkInstance;
		        a.fPhysicalDevice = vkContext->dh.vkPhysicalDevice;
		        a.fDevice = vkContext->dh.vkDevice;
		        a.fQueue = vkContext->dh.graphicQueue;
		        a.fGraphicsQueueIndex = vkContext->dh.graphicFamily;
		        a.fMaxAPIVersion = VK_MAKE_VERSION(1, 0, 0);
		        a.fVkExtensions = &vkExtension;
		        a.fGetProc = load_function;
		        a.fInstanceVersion = VK_MAKE_VERSION(1, 0, 0);
		
		
		        a.fFeatures = 0;
		        a.fDeviceFeatures = &vkContext->dh.features;
		        auto grContext = GrContext::MakeVulkan(a);
		        assert(grContext);
				return grContext!= nullptr;
		
		        //VkPhysicalDeviceMemoryProperties memoryProperties;
		        //uint32_t imageBufferIndex = (std::numeric_limits<uint32_t>::max)();
		        //uint32_t mappingBufferIndex = (std::numeric_limits<uint32_t>::max)();
		        //VkDeviceSize imageBufferSize = 0;
		        //VkDeviceSize mappinngBufferSize = 0;
		
		        //vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);
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

		} while (false);
		return rst;
	}
	//************************************
	// Method:    makeCanvas
	// FullName:  seraphim::SkiaBackedVK::makeCanvas
	// Access:    public 
	// Returns:   SkCanvas*
	// Qualifier:
	// Parameter: const string & t
	// Parameter: uint32_t w
	// Parameter: uint32_t h
	// Parameter: VkFormat ft
	// Parameter: VkColorSpaceKHR cs
	//************************************
	unique_ptr<SkCanvas> SkiaBackedVK::makeCanvas(const string& t, uint32_t w, uint32_t h, VkFormat ft , VkColorSpaceKHR  cs)
	{

		VkResult vkResult;
		VkImage image;
		VkDeviceMemory imageMemory=VK_NULL_HANDLE;
		VkDeviceMemory locateMemory  =  VK_NULL_HANDLE;
		VkDevice vkDevice = vkContext->dh.vkDevice;
		
		VkDeviceSize imageMemorySize = 0;
		//initMemory

		



		VkImageCreateInfo imageCreateInfo;

		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = nullptr;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = ft;
		imageCreateInfo.extent.width = w;
		imageCreateInfo.extent.height = h;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.queueFamilyIndexCount = 0; 
		imageCreateInfo.pQueueFamilyIndices = nullptr;//&a.fGraphicsQueueIndex;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		vkResult = vkCreateImage(vkContext->dh.vkDevice, &imageCreateInfo, nullptr, &image);
		assert(vkResult == VK_SUCCESS);


		VkMemoryRequirements imageRequirements;
		vkGetImageMemoryRequirements(vkDevice, image, &imageRequirements);
		imageMemorySize = imageRequirements.size * 64;
		VkMemoryAllocateInfo allocateInfo;
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.allocationSize = imageMemorySize;
		allocateInfo.memoryTypeIndex =  vkContext->dh.diviceMemoryIndex;
		vkResult = vkAllocateMemory(vkDevice, &allocateInfo, nullptr, &imageMemory);
		assert(vkResult == VK_SUCCESS);


		VkBindImageMemoryInfo bindInfo;
		bindInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
		bindInfo.pNext = nullptr;
		bindInfo.image = image;
		bindInfo.memory = imageMemory;
		bindInfo.memoryOffset = 0;
		PFN_vkBindImageMemory2 myBindImageMemory2 = (PFN_vkBindImageMemory2)vkGetDeviceProcAddr(vkDevice, "vkBindImageMemory2");
		vkResult = myBindImageMemory2(vkDevice, 1, &bindInfo);
		//vkResult = vkBindImageMemory(vkDevice, image, imageMemory, 0);



		assert(vkResult == VK_SUCCESS);
		GrVkAlloc grVkAlloc(imageMemory, 0, imageMemorySize, GrVkAlloc::kNoncoherent_Flag);
		GrVkImageInfo grImageInfo(image, grVkAlloc, VK_IMAGE_TILING_LINEAR,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_FORMAT_R8G8B8A8_SRGB, 1);
		GrBackendTexture grBackedTexture(w, h, grImageInfo);
		sk_sp<SkColorSpace> colorSpace = SkColorSpace::MakeSRGB();
		SkSurfaceProps *surfaceProps = nullptr;
		sk_sp<SkSurface> surface = SkSurface::MakeFromBackendTexture(grContext.get(),
			grBackedTexture,
			GrSurfaceOrigin::kTopLeft_GrSurfaceOrigin,
			1,
			SkColorType::kRGBA_8888_SkColorType,
			colorSpace,
			surfaceProps);
		assert(surface.get());
		unique_ptr<SkCanvas> pCanvas(surface->getCanvas());
		//this->backend = std::make_shared<VulkanSkiaBackend>(width, height, buffer, surface, image, 0, 0);
		return pCanvas;
	}

}

