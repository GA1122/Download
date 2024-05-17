void GLES2DecoderImpl::DoTexParameterf(
    GLenum target, GLenum pname, GLfloat param) {
  TextureRef* texture = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glTexParameterf", "unknown texture");
    return;
  }

  texture_manager()->SetParameterf("glTexParameterf", error_state_.get(),
                                   texture, pname, param);
}
