GLES2DecoderPassthroughImpl::DoRenderbufferStorageMultisampleCHROMIUM(
    GLenum target,
    GLsizei samples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height) {
  DCHECK(feature_info_->feature_flags().chromium_framebuffer_multisample);
  api()->glRenderbufferStorageMultisampleFn(target, samples, internalformat,
                                            width, height);
  return error::kNoError;
}
