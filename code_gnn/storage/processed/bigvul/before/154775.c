GLES2DecoderPassthroughImpl::DoRenderbufferStorageMultisampleAdvancedAMD(
    GLenum target,
    GLsizei samples,
    GLsizei storageSamples,
    GLenum internalformat,
    GLsizei width,
    GLsizei height) {
  DCHECK(feature_info_->feature_flags().amd_framebuffer_multisample_advanced);
  api()->glRenderbufferStorageMultisampleAdvancedAMDFn(
      target, samples, storageSamples, internalformat, width, height);
  return error::kNoError;
}
