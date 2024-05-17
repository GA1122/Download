SkiaOutputSurfaceImpl::CreateImageContext(
    const gpu::MailboxHolder& holder,
    const gfx::Size& size,
    ResourceFormat format,
    const base::Optional<gpu::VulkanYCbCrInfo>& ycbcr_info,
    sk_sp<SkColorSpace> color_space) {
  return std::make_unique<ImageContextImpl>(holder, size, format, ycbcr_info,
                                            std::move(color_space));
}
