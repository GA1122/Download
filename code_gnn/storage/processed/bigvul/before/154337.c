void GLES2DecoderImpl::RenderbufferStorageMultisampleWithWorkaround(
    GLenum target,
    GLsizei samples,
    GLenum internal_format,
    GLsizei width,
    GLsizei height,
    ForcedMultisampleMode mode) {
  RegenerateRenderbufferIfNeeded(state_.bound_renderbuffer.get());
  EnsureRenderbufferBound();
  RenderbufferStorageMultisampleHelper(target, samples, internal_format, width,
                                       height, mode);
}
