void GLES2DecoderImpl::RestoreTextureState(unsigned service_id) const {
  Texture* texture = texture_manager()->GetTextureForServiceId(service_id);
  if (texture) {
    GLenum target = texture->target();
    glBindTexture(target, service_id);
    glTexParameteri(
        target, GL_TEXTURE_WRAP_S, texture->wrap_s());
    glTexParameteri(
        target, GL_TEXTURE_WRAP_T, texture->wrap_t());
    glTexParameteri(
        target, GL_TEXTURE_MIN_FILTER, texture->min_filter());
    glTexParameteri(
        target, GL_TEXTURE_MAG_FILTER, texture->mag_filter());
    RestoreTextureUnitBindings(state_.active_texture_unit);
  }
}
