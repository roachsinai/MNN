//
//  VulkanImage.hpp
//  MNN
//
//  Created by MNN on 2019/01/31.
//  Copyright © 2018, Alibaba Group Holding Limited
//

#ifndef VulkanImage_hpp
#define VulkanImage_hpp
#include <MNN/Tensor.hpp>
#include "backend/vulkan/component/VulkanBuffer.hpp"
#include "backend/vulkan/component/VulkanMemoryPool.hpp"
namespace MNN {
class VulkanSampler : public NonCopyable {
public:
    VulkanSampler(const VulkanDevice& dev, VkFilter filter = VK_FILTER_NEAREST,
                  VkSamplerAddressMode mode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
    virtual ~VulkanSampler();

    VkSampler get() const {
        return mSampler;
    }
private:
    VkSampler mSampler;
    const VulkanDevice& mDevice;
};

class VulkanImage : public NonCopyable {
public:
    VulkanImage(const VulkanMemoryPool& pool, bool separate, const std::vector<int>& dims,
                halide_type_t type = halide_type_of<float>());
    VulkanImage(const VulkanMemoryPool& pool, bool separate, int w, int h)
        : VulkanImage(pool, separate, std::vector<int>{w, h}) {
    }
    virtual ~VulkanImage();

    inline int width() const {
        return std::get<1>(mInfo);
    }
    inline int height() const {
        return std::get<2>(mInfo);
    }
    inline int depth() const {
        return std::get<3>(mInfo);
    }
    inline std::vector<int> dims() const {
        return mDims;
    }
    inline VkImage get() const {
        return mImage.first;
    }
    inline VkImageView view() const {
        return mImage.second;
    }
    inline VkFormat format() const {
        return std::get<4>(mInfo);
    }
    void release();
    void resetBarrier() {
        mLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    }
    VkImageLayout currentLayout() const {
        return mLayout;
    }
    void barrierWrite(VkCommandBuffer buffer) const;
    void barrierRead(VkCommandBuffer buffer) const;
private:
    std::tuple<VkImageType, uint32_t, uint32_t, uint32_t, VkFormat> mInfo;
    std::pair<VkImage, VkImageView> mImage;
    const VulkanDevice& mDevice;
    std::vector<int> mDims;
    const VulkanMemoryPool& mPool;
    MemChunk mMemory;
    mutable VkImageLayout mLayout;
    mutable VkAccessFlagBits mAccess;
};
} // namespace MNN

#endif /* VulkanImage_hpp */
