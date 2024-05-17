void GLES2DecoderImpl::RestoreCurrentTexture2DBindings() {
  GLES2DecoderImpl::TextureUnit& info = texture_units_[0];
  GLuint last_id;
  if (info.bound_texture_2d) {
    last_id = info.bound_texture_2d->service_id();
  } else {
    last_id = 0;
  }

  glBindTexture(GL_TEXTURE_2D, last_id);
  glActiveTexture(GL_TEXTURE0 + active_texture_unit_);
}
