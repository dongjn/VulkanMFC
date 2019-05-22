#ifndef SKIA_BACKED_CONTEXT
#define SKIA_BACKED_CONTEXT
#include <SkCanvas.h>
#include<SkSurface.h>
#include"vulkan_context.h"
#include<gpu/GrContext.h>
#include<gpu/vk/GrVkExtensions.h>
#include<gpu/vk/GrVkMemoryAllocator.h>
#include<gpu/vk/GrVkBackendContext.h>
#include<gpu/vk/GrVkVulkan.h>
#include<memory>
#include<string>
class GrVkExtensions;

using std::shared_ptr;
using std::unique_ptr;
using std::string;
namespace seraphim {
	class SkiaBackedVK {
	private:
		struct BackendHandle{
			const string tag;
			VkImage  image{VK_NULL_HANDLE};
			VkDeviceMemory imageMemory{VK_NULL_HANDLE};
			VkDeviceMemory localMemory{VK_NULL_HANDLE};
			uint32_t width{(std::numeric_limits<uint32_t>::max)()};
			uint32_t height{(std::numeric_limits<uint32_t>::max)()};
			VkFormat format;
			VkColorSpaceKHR colorSpace;
		};
	public:
		static shared_ptr<SkiaBackedVK> get();
		static shared_ptr<SkiaBackedVK> make(shared_ptr<VulkanContext> vkContext);
	private:
		
		shared_ptr<VulkanContext> vkContext;
		unique_ptr<GrVkExtensions> grVkExtensions;
		unique_ptr<GrContext> grContext;
		static shared_ptr<SkiaBackedVK> self;
		SkiaBackedVK(shared_ptr<VulkanContext> context) :vkContext(context) {

		}
		bool init();
	public:// Toolkit

	public:
		unique_ptr<SkCanvas> SkiaBackedVK::makeCanvas(const string& t, uint32_t w, uint32_t h, VkFormat ft = VK_FORMAT_B8G8R8A8_UINT, VkColorSpaceKHR  cs = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);

		
	};

}


#endif
