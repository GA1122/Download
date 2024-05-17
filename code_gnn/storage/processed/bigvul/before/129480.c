void GLES2DecoderImpl::RenderbufferStorageMultisampleHelper(
    const FeatureInfo* feature_info,
    GLenum target,
    GLsizei samples,
    GLenum internal_format,
    GLsizei width,
    GLsizei height) {
  if (feature_info->feature_flags().is_angle) {
    glRenderbufferStorageMultisampleANGLE(
        target, samples, internal_format, width, height);
  } else if (feature_info->feature_flags().use_core_framebuffer_multisample) {
    glRenderbufferStorageMultisample(
        target, samples, internal_format, width, height);
  } else {
    glRenderbufferStorageMultisampleEXT(
        target, samples, internal_format, width, height);
  }
}
