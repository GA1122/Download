void GLES2DecoderImpl::DoTexParameterfv(GLenum target,
                                        GLenum pname,
                                        const volatile GLfloat* params) {
  TextureRef* texture = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glTexParameterfv", "unknown texture");
    return;
  }

  texture_manager()->SetParameterf("glTexParameterfv", error_state_.get(),
                                   texture, pname, *params);
}