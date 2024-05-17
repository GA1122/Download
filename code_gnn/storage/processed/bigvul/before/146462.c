void WebGLRenderingContextBase::colorMask(GLboolean red,
                                          GLboolean green,
                                          GLboolean blue,
                                          GLboolean alpha) {
  if (isContextLost())
    return;
  color_mask_[0] = red;
  color_mask_[1] = green;
  color_mask_[2] = blue;
  color_mask_[3] = alpha;
  ContextGL()->ColorMask(red, green, blue, alpha);
}
