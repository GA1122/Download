size_t GLES2DecoderPassthroughImpl::GetSavedBackTextureCountForTest() {
  return in_use_color_textures_.size() + available_color_textures_.size();
}
