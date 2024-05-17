void WebGLRenderingContextBase::RestoreColorMask() {
  if (isContextLost())
    return;

  ContextGL()->ColorMask(color_mask_[0], color_mask_[1], color_mask_[2],
                         color_mask_[3]);
}
