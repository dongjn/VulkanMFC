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
namespace seraphim{
    struct VulkanSkiaBackend{

        uint32_t width{0};
        uint32_t height{0};
        uint8_t* dstBuffer{nullptr};
        sk_sp<SkSurface>  surface{nullptr};
        SkCanvas*   canvas;
        VkImage  vkImage;
        VkDeviceSize  deviceMemorySize;
        VkDeviceSize  deviceMemoryOffset;
        SkCanvas* getCanvas(){
            canvas->resetMatrix();
            return canvas;
        }

        ~VulkanSkiaBackend(){
            delete canvas;
        }
        VulkanSkiaBackend(uint32_t w,uint32_t h,uint8_t* b,sk_sp<SkSurface> s,VkImage i, VkDeviceSize deviceSize,VkDeviceSize deviceOffset);
    };


    struct VulkanSurfaceContext{
        VkSurfaceKHR  surface;
        VkSwapchainKHR  swapchainKHR;
        VkQueue graphicQueue;
        uint32_t graphicFamilyIndex;
        std::vector<VkImage> swapChainImages;
    };


    class VulkanContext{
    public:
        static std::shared_ptr<VulkanContext> make(HWND window, HINSTANCE hInstance, UINT32 width, UINT32 height){
           if(gpVKContext)
               return gpVKContext;
           gpVKContext.reset(new VulkanContext(window,hInstance,width,height));
           gpVKContext->initVulkan();
           //gpVKContext->initGrContext();
           return gpVKContext;
        } ;

		static std::shared_ptr<VulkanContext> get() {
			//assert(gpVKContext);
			return gpVKContext;
		}
    private:

		//global
		static std::shared_ptr<VulkanContext> gpVKContext;
		std::vector<const char*> vkDeviceExtensionName{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		std::vector<const char*> vkInstanceExtensionsName { VK_KHR_SURFACE_EXTENSION_NAME, "VK_KHR_win32_surface",VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
		std::vector<const char*> vkLayerExtensionName;
		//VK_KHR_SWAPCHAIN_EXTENSION_NAM
		VkDebugReportCallbackEXT debugReportCallbackExt;
		//Skia
		shared_ptr<VulkanSkiaBackend>  backend;
		sk_sp<GrContext> grContext;
		VkDeviceMemory  skiaImageMemory;
		VkDeviceSize    skiaImageMemorySize;
		VkDeviceSize    skiaImageMemoryOffset;
		VkBuffer        copyBuffer;
		VkDeviceMemory  copyMemory;



		//Vulkan
		VkSurfaceKHR  surface;
		VkSwapchainKHR swapchain{VK_NULL_HANDLE};
        VkInstance vkInstance;
        VkDevice  vkDevice;
        VkFence submitFence;
        VkPhysicalDevice vkPhysicalDevice;
        HWND window{nullptr};
		HINSTANCE hInstance{ nullptr };
		UINT wWidth;
		UINT wHeight;
		HINSTANCE vulkanInstance;
        VkDeviceMemory  localMemory;
        VkBuffer        localBuffer;
        uint32_t vulkan_api{0};
//        VkQueue
        VkQueue  graphicQueue;
        VkQueue  transferQueue;
        VkQueue  calculateQueue;
        uint32_t graphicFamilyIndex{(std::numeric_limits<uint32_t>::max)()};
        uint32_t calculateFamilyIndex{(std::numeric_limits<uint32_t >::max)()};
        uint32_t transferFamilyIndex{(std::numeric_limits<uint32_t>::max)()};

        VkPhysicalDeviceFeatures features;

        VkCommandPool   transferCommandPool;
        VkCommandBuffer transferCmdBuffer;
		VkCommandPool  graphicCommandPool;
		VkCommandBuffer graphicCmdBuffer;




        //SwapChain
        unique_ptr<VulkanSurfaceContext> surfaceContext;
    public:

        VulkanContext(HWND window,HINSTANCE hInstance ,UINT32 width,UINT32 height);

        void commit();

        SkCanvas* getCanvas(){
            if(backend.get()){
                return backend->surface->getCanvas();
            }
            return nullptr;
        }
        void testRead();
        SkCanvas* makeBackend(uint32_t  width, uint32_t height,uint8_t* buffer);

		void resize(UINT32 width, UINT32 height);
		void clean();

    ~VulkanContext();
    private:
        void initVulkan();
        void initGrContext();

    };
};

#endif