sk_sp<SkImage> SkiaOutputSurfaceImpl::MakePromiseSkImageFromRenderPass(
    const RenderPassId& id,
    const gfx::Size& size,
    ResourceFormat format,
    bool mipmap,
    sk_sp<SkColorSpace> color_space) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(current_paint_);

  auto& image_context = render_pass_image_cache_[id];
  if (!image_context) {
    image_context = std::make_unique<ImageContextImpl>(id, size, format, mipmap,
                                                       std::move(color_space));
  }
  if (!image_context->has_image()) {
    SkColorType color_type =
        ResourceFormatToClosestSkColorType(true  , format);
    GrBackendFormat backend_format = GetGrBackendFormatForTexture(
        format, GL_TEXTURE_2D,  base::nullopt);
    image_context->SetImage(
        current_paint_->recorder()->makePromiseTexture(
            backend_format, image_context->size().width(),
            image_context->size().height(), image_context->mipmap(),
            image_context->origin(), color_type, image_context->alpha_type(),
            image_context->color_space(), Fulfill, DoNothing, DoNothing,
            image_context.get()),
        backend_format);
    DCHECK(image_context->has_image());
  }
  images_in_current_paint_.push_back(image_context.get());
  return image_context->image();
}
