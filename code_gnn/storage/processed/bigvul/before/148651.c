void SkiaOutputSurfaceImpl::MakePromiseSkImage(ImageContext* image_context) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(current_paint_);
  DCHECK(!image_context->mailbox_holder().mailbox.IsZero());

  images_in_current_paint_.push_back(
      static_cast<ImageContextImpl*>(image_context));

  if (image_context->has_image())
    return;

  SkColorType color_type = ResourceFormatToClosestSkColorType(
      true  , image_context->resource_format());
  GrBackendFormat backend_format = GetGrBackendFormatForTexture(
      image_context->resource_format(),
      image_context->mailbox_holder().texture_target,
      image_context->ycbcr_info());
  image_context->SetImage(
      current_paint_->recorder()->makePromiseTexture(
          backend_format, image_context->size().width(),
          image_context->size().height(), GrMipMapped::kNo,
          image_context->origin(), color_type, image_context->alpha_type(),
          image_context->color_space(), Fulfill  ,
          DoNothing  , DoNothing  ,
          image_context  ),
      backend_format);

  if (image_context->mailbox_holder().sync_token.HasData()) {
    resource_sync_tokens_.push_back(image_context->mailbox_holder().sync_token);
    image_context->mutable_mailbox_holder()->sync_token.Clear();
  }
}
