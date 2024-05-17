void GLES2DecoderImpl::RenderbufferStorageMultisampleHelperAMD(
    GLenum target,
    GLsizei samples,
    GLsizei storageSamples,
    GLenum internal_format,
    GLsizei width,
    GLsizei height,
    ForcedMultisampleMode mode) {
  api()->glRenderbufferStorageMultisampleAdvancedAMDFn(
      target, samples, storageSamples, internal_format, width, height);
}
