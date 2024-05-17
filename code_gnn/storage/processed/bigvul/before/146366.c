ScopedRGBEmulationColorMask::ScopedRGBEmulationColorMask(
    WebGLRenderingContextBase* context,
    GLboolean* color_mask,
    DrawingBuffer* drawing_buffer)
    : context_(context),
      requires_emulation_(drawing_buffer->RequiresAlphaChannelToBePreserved()) {
  if (requires_emulation_) {
    context_->active_scoped_rgb_emulation_color_masks_++;
    memcpy(color_mask_, color_mask, 4 * sizeof(GLboolean));
    context_->ContextGL()->ColorMask(color_mask_[0], color_mask_[1],
                                     color_mask_[2], false);
  }
}
