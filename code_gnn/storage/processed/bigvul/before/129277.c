void GLES2DecoderImpl::DoFramebufferTexture2D(
    GLenum target, GLenum attachment, GLenum textarget,
    GLuint client_texture_id, GLint level) {
  DoFramebufferTexture2DCommon(
    "glFramebufferTexture2D", target, attachment,
    textarget, client_texture_id, level, 0);
}
