error::Error GLES2DecoderPassthroughImpl::DoBlitFramebufferCHROMIUM(
    GLint srcX0,
    GLint srcY0,
    GLint srcX1,
    GLint srcY1,
    GLint dstX0,
    GLint dstY0,
    GLint dstX1,
    GLint dstY1,
    GLbitfield mask,
    GLenum filter) {
  DCHECK(feature_info_->feature_flags().chromium_framebuffer_multisample);
  api()->glBlitFramebufferFn(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1,
                             dstY1, mask, filter);
  return error::kNoError;
}
