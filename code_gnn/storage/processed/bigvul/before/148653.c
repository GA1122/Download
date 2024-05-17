sk_sp<SkImage> SkiaOutputSurfaceImpl::MakePromiseSkImageFromYUV(
    const std::vector<ImageContext*>& contexts,
    SkYUVColorSpace yuv_color_space,
    sk_sp<SkColorSpace> dst_color_space,
    bool has_alpha) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(current_paint_);
  DCHECK((has_alpha && (contexts.size() == 3 || contexts.size() == 4)) ||
         (!has_alpha && (contexts.size() == 2 || contexts.size() == 3)));

  SkYUVAIndex indices[4];
  PrepareYUVATextureIndices(contexts, has_alpha, indices);

  GrBackendFormat formats[4] = {};
  SkISize yuva_sizes[4] = {};
  SkDeferredDisplayListRecorder::PromiseImageTextureContext
      texture_contexts[4] = {};
  for (size_t i = 0; i < contexts.size(); ++i) {
    auto* context = static_cast<ImageContextImpl*>(contexts[i]);
    DCHECK(context->origin() == kTopLeft_GrSurfaceOrigin);
    formats[i] = GetGrBackendFormatForTexture(
        context->resource_format(), context->mailbox_holder().texture_target,
         base::nullopt);
    yuva_sizes[i].set(context->size().width(), context->size().height());

    context->SetImage(nullptr, formats[i]);

    if (context->mailbox_holder().sync_token.HasData()) {
      resource_sync_tokens_.push_back(context->mailbox_holder().sync_token);
      context->mutable_mailbox_holder()->sync_token.Clear();
    }
    images_in_current_paint_.push_back(context);
    texture_contexts[i] = context;
  }

  auto image = current_paint_->recorder()->makeYUVAPromiseTexture(
      yuv_color_space, formats, yuva_sizes, indices, yuva_sizes[0].width(),
      yuva_sizes[0].height(), kTopLeft_GrSurfaceOrigin, dst_color_space,
      Fulfill, DoNothing, DoNothing, texture_contexts);
  DCHECK(image);
  return image;
}
