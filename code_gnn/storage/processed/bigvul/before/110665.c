TextureManager::TextureInfo* TextureManager::CreateTextureInfo(
    GLuint client_id, GLuint service_id) {
  DCHECK_NE(0u, service_id);
  TextureInfo::Ref info(new TextureInfo(this, service_id));
  std::pair<TextureInfoMap::iterator, bool> result =
      texture_infos_.insert(std::make_pair(client_id, info));
  DCHECK(result.second);
  if (!info->CanRender(feature_info_)) {
    ++num_unrenderable_textures_;
  }
  if (!info->SafeToRenderFrom()) {
    ++num_unsafe_textures_;
  }
  num_uncleared_mips_ += info->num_uncleared_mips();
  return info.get();
}
