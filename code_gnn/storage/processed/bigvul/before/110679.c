void TextureManager::RemoveTextureInfo(GLuint client_id) {
  TextureInfoMap::iterator it = texture_infos_.find(client_id);
  if (it != texture_infos_.end()) {
    TextureInfo* info = it->second;
    info->MarkAsDeleted();
    texture_infos_.erase(it);
  }
}
