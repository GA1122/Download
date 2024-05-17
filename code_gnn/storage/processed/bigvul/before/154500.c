bool GLES2DecoderPassthroughImpl::EmulatedDefaultFramebuffer::Resize(
    const gfx::Size& new_size,
    const FeatureInfo* feature_info) {
  DCHECK(!new_size.IsEmpty());
  if (size == new_size) {
    return true;
  }
  size = new_size;

  if (color_buffer_service_id != 0) {
    ResizeRenderbuffer(api, color_buffer_service_id, size, format.samples,
                       format.color_renderbuffer_internal_format, feature_info);
  }
  if (color_texture)
    color_texture->Resize(size);
  if (depth_stencil_buffer_service_id != 0) {
    ResizeRenderbuffer(api, depth_stencil_buffer_service_id, size,
                       format.samples, format.depth_stencil_internal_format,
                       feature_info);
  }
  if (depth_buffer_service_id != 0) {
    ResizeRenderbuffer(api, depth_buffer_service_id, size, format.samples,
                       format.depth_internal_format, feature_info);
  }
  if (stencil_buffer_service_id != 0) {
    ResizeRenderbuffer(api, stencil_buffer_service_id, size, format.samples,
                       format.stencil_internal_format, feature_info);
  }

  {
    ScopedFramebufferBindingReset scoped_fbo_reset(
        api, supports_separate_fbo_bindings);
    api->glBindFramebufferEXTFn(GL_FRAMEBUFFER, framebuffer_service_id);
    if (api->glCheckFramebufferStatusEXTFn(GL_FRAMEBUFFER) !=
        GL_FRAMEBUFFER_COMPLETE) {
      LOG(ERROR)
          << "GLES2DecoderPassthroughImpl::ResizeOffscreenFramebuffer failed "
          << "because the resulting framebuffer was not complete.";
      return false;
    }
  }

  DCHECK(color_texture == nullptr || color_texture->size == size);

  return true;
}
