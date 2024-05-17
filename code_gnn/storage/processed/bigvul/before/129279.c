void GLES2DecoderImpl::DoFramebufferTexture2DMultisample(
    GLenum target, GLenum attachment, GLenum textarget,
    GLuint client_texture_id, GLint level, GLsizei samples) {
  DoFramebufferTexture2DCommon(
    "glFramebufferTexture2DMultisample", target, attachment,
    textarget, client_texture_id, level, samples);
}
