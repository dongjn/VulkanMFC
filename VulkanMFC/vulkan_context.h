#ifndef VULKAN_ANDROID_CONTEXT_H
#define VULKAN_ANDROID_CONTEXT_H
#include<vulkan/vulkan.h>
//#include<vulkan/vulkan_win32.h>
#include<memory>
#include<vector>
#include <SkCanvas.h>
#include<SkSurface.h>
#include<SkRefCnt.h>
#include<vector>
#include<Windows.h>
class SkCanvas;
class SkSurface;

using  std::shared_ptr;
using std::unique_ptr;
namespace seraphim {
	struct VulkanSkiaBackend {

		uint32_t width{ 0 };
		uint32_t height{ 0 };
		uint8_t* dstBuffer{ nullptr };
		sk_sp<SkSurface>  surface{ nullptr };
		SkCanvas*   canvas;
		VkImage  vkImage;
		VkDeviceSize  deviceMemorySize;
		VkDeviceSize  deviceMemoryOffset;
		SkCanvas* getCanvas() {
			canvas->resetMatrix();
			return canvas;
		}

		~VulkanSkiaBackend() {
			delete canvas;
		}
		VulkanSkiaBackend(uint32_t w, uint32_t h, uint8_t* b, sk_sp<SkSurface> s, VkImage i, VkDeviceSize deviceSize, VkDeviceSize deviceOffset);
	};


	struct VulkanSurfaceContext {
		VkSurfaceKHR  surface;
		VkSwapchainKHR  swapchainKHR;
		VkQueue graphicQueue;
		uint32_t graphicFamilyIndex;
		std::vector<VkImage> swapChainImages;
	};


	class VulkanContext {
	private:
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		struct InstanceHandle {
			std::vector<const char*> vkInstanceExtensionsName;
			std::vector<const char*> vkLayerExtensionName;
			HINSTANCE hInstance{ nullptr };
			VkInstance vkInstance{ VK_NULL_HANDLE };
			VkDebugReportCallbackEXT debugReportCallbackExt;
		};
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		struct DeviceHandle {
			std::vector<const char*> vkDeviceExtensionName;
			VkPhysicalDevice vkPhysicalDevice{ VK_NULL_HANDLE };
			VkDevice  vkDevice{ VK_NULL_HANDLE };
			VkFence submitFence{ VK_NULL_HANDLE };
			VkQueue  graphicQueue{ VK_NULL_HANDLE };
			VkQueue  transferQueue{ VK_NULL_HANDLE };
			VkQueue  calculateQueue{ VK_NULL_HANDLE };
			VkCommandPool graphicPool{VK_NULL_HANDLE};
			VkCommandPool transferPool{ VK_NULL_HANDLE};
			VkCommandPool calculatePool{VK_NULL_HANDLE};
			VkPhysicalDeviceFeatures features;
			uint32_t graphicFamily{ (std::numeric_limits<uint32_t>::max)() };
			uint32_t calculateFamily{ (std::numeric_limits<uint32_t >::max)() };
			uint32_t transferFamily{ (std::numeric_limits<uint32_t>::max)() };

		};
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		struct PresentHandle {
			VkSwapchainKHR swapchain;
			VkSurfaceKHR  surface;
			VkFormat   format;
			VkColorSpaceKHR  colorSpace;
			VkFence acquireFence{VK_NULL_HANDLE};
			VkSemaphore acquireSemaphore{ VK_NULL_HANDLE };

			
			std::vector<VkImage> images;

			std::vector<VkCommandBuffer> imageCmdBuffer;
			uint32_t currentImageIndex;

		};
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		struct SkiaBackedHandle {


		};
	public:
		static std::shared_ptr<VulkanContext> make(HWND window, HINSTANCE hInstance, UINT32 width, UINT32 height) {
			if (gpVKContext)
				return gpVKContext;
			VulkanContext* c = new VulkanContext(window, hInstance, width, height);
			gpVKContext.reset(c);
			gpVKContext->initVulkan();
			//gpVKContext->initGrContext();
			return gpVKContext;
		};

		static std::shared_ptr<VulkanContext> get() {
			//assert(gpVKContext);
			return gpVKContext;
		}
	private:

		//global
		static std::shared_ptr<VulkanContext> gpVKContext;
		HWND window{ nullptr };
		HINSTANCE hInstance{ nullptr };
		UINT wWidth;
		UINT wHeight;
		InstanceHandle iHandle;
		DeviceHandle   dh;
		PresentHandle  ph;
		SkiaBackedHandle sh;



		//Skia
		HINSTANCE vulkanInstance;

		uint32_t vulkan_api{ 0 };
		//        VkQueue
		VkDeviceMemory  localMemory;
		VkBuffer        localBuffer;
		shared_ptr<VulkanSkiaBackend>  backend;
		sk_sp<GrContext> grContext;
		VkDeviceMemory  skiaImageMemory;
		VkDeviceSize    skiaImageMemorySize;
		VkDeviceSize    skiaImageMemoryOffset;
		VkBuffer        copyBuffer;
		VkDeviceMemory  copyMemory;

		//SwapChain
		unique_ptr<VulkanSurfaceContext> surfaceContext;
	public:

		VulkanContext(HWND window, HINSTANCE hInstance, UINT32 width, UINT32 height);

		void commit();

		SkCanvas* getCanvas() {
			if (backend.get()) {
				return backend->surface->getCanvas();
			}
			return nullptr;
		}
		void testRead();
		SkCanvas* makeBackend(uint32_t  width, uint32_t height, uint8_t* buffer);

		void resize(UINT32 width, UINT32 height);
		void cleanBank();
		void clean();
		void draw();
		~VulkanContext();
	private:
		void initVulkan();
		void initInstance();
		void initDevice();
		void initPresent();
		void initGrContext();

	};
};

#endif