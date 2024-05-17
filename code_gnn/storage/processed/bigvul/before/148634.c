SkiaOutputSurfaceImpl::CreateSkSurfaceCharacterization(
    const gfx::Size& surface_size,
    ResourceFormat format,
    bool mipmap,
    sk_sp<SkColorSpace> color_space) {
  auto gr_context_thread_safe = impl_on_gpu_->GetGrContextThreadSafeProxy();
  auto cache_max_resource_bytes = impl_on_gpu_->max_resource_cache_bytes();
  SkSurfaceProps surface_props(0  ,
                               SkSurfaceProps::kLegacyFontHost_InitType);
  auto color_type =
      ResourceFormatToClosestSkColorType(true  , format);
  auto image_info =
      SkImageInfo::Make(surface_size.width(), surface_size.height(), color_type,
                        kPremul_SkAlphaType, std::move(color_space));
  auto backend_format = gr_context_thread_safe->defaultBackendFormat(
      color_type, GrRenderable::kYes);
  DCHECK(backend_format.isValid());
  auto characterization = gr_context_thread_safe->createCharacterization(
      cache_max_resource_bytes, image_info, backend_format, 0  ,
      kTopLeft_GrSurfaceOrigin, surface_props, mipmap);
  DCHECK(characterization.isValid());
  return characterization;
}
