#pragma once
#include<vulkan.h>
namespace seraphim{

	//************************************
	// Method:    create_image_barrier
	// FullName:  seraphim::create_image_barrier
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: VkImage image
	// Parameter: VkImageAspectFlags aspectMask
	// Parameter: VkImageLayout old_image_layout
	// Parameter: VkImageLayout new_image_layout
	// Parameter: VkImageMemoryBarrier & dstBarrier
	//************************************
	void set_image_barrier(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout old_image_layout, VkImageLayout new_image_layout, VkImageMemoryBarrier& dstBarrier);
};
