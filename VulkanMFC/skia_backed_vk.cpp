#include "skia_backed_vk.h"
#include<vulkan.h>
#include<set>
#include<assert.h>
#include"utility.h"
#include"vulkan_utility.h"
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
	

			VkCommandPoolCreateInfo commandPoolInfo;
			commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			commandPoolInfo.pNext = nullptr;
			commandPoolInfo.queueFamilyIndex = vkContext->dh.transferFamily;
			commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			VkResult vkResult = vkCreateCommandPool(vkContext->dh.vkDevice, &commandPoolInfo, nullptr, &commandPool);
			assert(VK_SUCCESS == vkResult);
			VkCommandBufferAllocateInfo allocateInfo;
			allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocateInfo.pNext = nullptr;
			allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocateInfo.commandPool = commandPool;
			allocateInfo.commandBufferCount = 1;
			vkResult = vkAllocateCommandBuffers(vkContext->dh.vkDevice, &allocateInfo, &commandBuffer);
			assert(VK_SUCCESS == vkResult);

			VkFenceCreateInfo fenceInfo{
				VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				nullptr,
				VK_FENCE_CREATE_SIGNALED_BIT
			};

			vkResult = vkCreateFence(vkContext->dh.vkDevice, &fenceInfo, nullptr, &submintFence);
			assert(VK_SUCCESS == vkResult);
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
		        grContext = GrContext::MakeVulkan(a);
				
		  //      assert(gc);
				//grContext.reset(gc.get());
				//gc.reset(nullptr);
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
	unique_ptr<SkCanvas> SkiaBackedVK::makeBacked(int tag, uint32_t w, uint32_t h, VkFormat ft , VkColorSpaceKHR  cs)
	{

		VkResult vkResult;
		VkImage image;
		VkDeviceMemory imageMemory=VK_NULL_HANDLE;
		VkDeviceMemory locateMemory  =  VK_NULL_HANDLE;
		VkBuffer localBuffer = VK_NULL_HANDLE;
		VkDevice vkDevice = vkContext->dh.vkDevice;
		
		VkDeviceSize imageMemorySize = 0;
		//CreateImage
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
		




		//BindImageMemory
		VkMemoryRequirements imageRequirements;
		vkGetImageMemoryRequirements(vkDevice, image, &imageRequirements);
		imageMemorySize = imageRequirements.size  * 4 ;
		VkMemoryAllocateInfo allocateInfo;
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.allocationSize = imageMemorySize;
		allocateInfo.memoryTypeIndex = vkContext->queryMemoryTypeIndex(imageRequirements.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
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

		//local handle
		//VkImage copyImage;
		//VkImageCreateInfo copyImageInfo;
		//copyImageInfo.arrayLayers = 1;
		//copyImageInfo.extent = { w,h,1 };
		//copyImageInfo.flags = 0;
		//copyImageInfo.format = ft;
		//copyImageInfo.imageType = VK_IMAGE_TYPE_2D;
		//copyImageInfo.initialLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		//copyImageInfo.mipLevels = 0;
		//copyImageInfo.pNext = nullptr;
		//copyImageInfo.pQueueFamilyIndices = nullptr;
		//copyImageInfo.queueFamilyIndexCount = 0;
		//copyImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		//copyImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//copyImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		//copyImageInfo.tiling = VK_IMAGE_TILING_LINEAR;
		//copyImageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		//vkResult =vkCreateImage(vkContext->dh.vkDevice, &copyImageInfo, nullptr, &copyImage);
		//assert(VK_SUCCESS == vkResult);
		//VkMemoryRequirements copyMemoryRequirements;
		//vkGetImageMemoryRequirements(vkContext->dh.vkDevice, image, &copyMemoryRequirements);
		//VkMemoryAllocateInfo copyMemoryAllocateInfo;
		//copyMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//copyMemoryAllocateInfo.pNext = nullptr;
		//copyMemoryAllocateInfo.memoryTypeIndex = vkContext->queryMemoryTypeIndex(copyMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		//copyMemoryAllocateInfo.allocationSize = copyMemoryRequirements.size;
		//vkResult = vkAllocateMemory(vkDevice, &copyMemoryAllocateInfo, nullptr,&locateMemory);
		//assert(VK_SUCCESS == vkResult);
		//vkResult = vkBindImageMemory(vkDevice, copyImage, locateMemory, 0);
		//assert(VK_SUCCESS == vkResult);



		//bufBindInfo.memoryOffset = 0;
		//bufBindInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
		//bufBindInfo.pNext = nullptr;
		//bufBindInfo.image = image;
		//bufBindInfo.memory = imageMemory;
		

		//Make GrBackendText SkSurface SkCanvas
		assert(vkResult == VK_SUCCESS);
		//GrVkAlloc grVkAlloc(VK_NULL_HANDLE, 0, imageMemorySize, GrVkAlloc::kNoncoherent_Flag);
		GrVkAlloc grVkAlloc(imageMemory,imageRequirements.size, imageMemorySize - imageRequirements.size, GrVkAlloc::kNoncoherent_Flag);
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
		std::shared_ptr<BackendHandle> handle = std::make_shared<BackendHandle>();
		handle->tag = tag;
		handle->colorSpace = cs;
		handle->format = ft;
		handle->height = h;
		handle->width = w;
		handle->image = image;
		handle->imageMemory = imageMemory;
		handle->localMemory = locateMemory;
		//handle->localBuffer = localBuffer;
		handle->surface.swap(surface);
		mapBackend.insert(std::make_pair(tag, handle));
		return pCanvas;
	}

	unique_ptr<SkCanvas> SkiaBackedVK::resizeBacked(int tag, uint32_t width, uint32_t height)
	{
		return nullptr;
	}

	void SkiaBackedVK::releaseBacked(int tag)
	{

	}

	size_t SkiaBackedVK::readPixel(int tag, uint8_t* buf, size_t cbBuffer)
	{
		auto itr = mapBackend.find(tag);
		if (itr == mapBackend.end()) {
			return 0;
		}
		auto ph = itr->second;
		size_t cbData = ph->width * ph->height * 4;

		sk_sp<SkColorSpace> colorSpace = SkColorSpace::MakeSRGB();

		auto dstImgInfo = SkImageInfo::Make(ph->width, ph->height,
			SkColorType::kRGBA_8888_SkColorType,
			SkAlphaType::kPremul_SkAlphaType, colorSpace);
		SkBitmap dstBitmap = SkBitmap();//(dstImgInfo);
		dstBitmap.setInfo(dstImgInfo);
		dstBitmap.setPixels(buf);
		slog(INFO_LEVEL, TAG, "readPiexl---0");
		bool bRst = ph->surface->readPixels(dstBitmap, 0, 0);
		slog(INFO_LEVEL, TAG, "readPiexl---1");
		return cbData;
	}
	//size_t SkiaBackedVK::readPixel(int tag, uint8_t* buf, size_t cbBuffer)
	//{
	//	VkResult vkResult;
	//	auto itr = mapBackend.find(tag);
	//	//auto buffer = itr->second->localBuffer;
	//	VkImage copyImage;
	//	auto local_memory = itr->second->localMemory;
	//	auto image = itr->second->image;
	//	auto width = itr->second->width;
	//	auto height = itr->second->height;

	//	VkCommandBufferBeginInfo beginInfo;
	//	beginInfo.pInheritanceInfo = NULL;
	//	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//	beginInfo.pNext = nullptr;
	//	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	//	VkImageMemoryBarrier imageBarrier;
	//	set_image_barrier(image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, imageBarrier);
	//	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier);
	//	VkBufferImageCopy regions;
	//	regions.bufferImageHeight = height;
	//	regions.bufferOffset = 0;
	//	regions.bufferRowLength =  0;
	//	regions.imageExtent.width = width;
	//	regions.imageExtent.height = height;
	//	regions.imageExtent.depth = 1;
	//	regions.imageOffset = { 0,0,0 };
	//	regions.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//	regions.imageSubresource.baseArrayLayer = 0;
	//	regions.imageSubresource.layerCount = 1;
	//	regions.imageSubresource.mipLevel = 0;
	//	//vkCmdCopyImageToBuffer(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer, 1, &regions);
	//	set_image_barrier(image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, imageBarrier);
	//	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier);
	//	 vkResult = vkEndCommandBuffer(commandBuffer);
	//	assert(vkResult == VK_SUCCESS);
	//	
	//	vkResetFences(vkContext->dh.vkDevice, 1, &submintFence);
	//	VkSubmitInfo submitInfo;
	//	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//	submitInfo.pNext = nullptr;
	//	submitInfo.commandBufferCount = 1;
	//	submitInfo.pCommandBuffers = &commandBuffer;
	//	submitInfo.pSignalSemaphores = nullptr;
	//	submitInfo.signalSemaphoreCount = 0;
	//	submitInfo.pWaitSemaphores = nullptr;
	//	submitInfo.waitSemaphoreCount = 0;
	//	submitInfo.pWaitDstStageMask = nullptr;
	//	VkFence fence;
	//	VkFenceCreateInfo fenceInfo;
	//	fenceInfo.flags = 0;
	//	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//	fenceInfo.pNext = nullptr;
	//	vkResult = vkCreateFence(vkContext->dh.vkDevice, &fenceInfo, nullptr, &fence);
	//	//VkSubmitInfo(vkContext->dh.transferQueue, &submitInfo);
	//	vkResult = vkQueueSubmit(vkContext->dh.transferQueue, 1, &submitInfo,fence);
	//	assert(vkResult == VK_SUCCESS);
	//	vkResult = vkWaitForFences(vkContext->dh.vkDevice, 1, &fence, VK_TRUE, 10000);
	//	assert(vkResult == VK_SUCCESS);
	//	uint8_t* pData = nullptr;
	//	vkResult =vkMapMemory(vkContext->dh.vkDevice, local_memory, 0, width* height * 4, 0, (void**)&pData);
	//	assert(vkResult == VK_SUCCESS);
	//	memcpy(buf, pData, cbBuffer);
	//	vkUnmapMemory(vkContext->dh.vkDevice, local_memory);
	//	return cbBuffer;
	//}
}


