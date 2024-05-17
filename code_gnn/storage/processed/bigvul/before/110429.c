void GLES2DecoderImpl::BindAndApplyTextureParameters(
    TextureManager::TextureInfo* info) {
  glBindTexture(info->target(), info->service_id());
  glTexParameteri(info->target(), GL_TEXTURE_MIN_FILTER, info->min_filter());
  glTexParameteri(info->target(), GL_TEXTURE_MAG_FILTER, info->mag_filter());
  glTexParameteri(info->target(), GL_TEXTURE_WRAP_S, info->wrap_s());
  glTexParameteri(info->target(), GL_TEXTURE_WRAP_T, info->wrap_t());
}
