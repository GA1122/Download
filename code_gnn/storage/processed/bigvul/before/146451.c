void WebGLRenderingContextBase::blendFuncSeparate(GLenum src_rgb,
                                                  GLenum dst_rgb,
                                                  GLenum src_alpha,
                                                  GLenum dst_alpha) {
  if (isContextLost() ||
      !ValidateBlendFuncFactors("blendFuncSeparate", src_rgb, dst_rgb))
    return;
  ContextGL()->BlendFuncSeparate(src_rgb, dst_rgb, src_alpha, dst_alpha);
}
