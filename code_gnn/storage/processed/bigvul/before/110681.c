TextureDefinition* TextureManager::Save(TextureInfo* info) {
  DCHECK(info->owned_);

  if (info->IsAttachedToFramebuffer())
    return NULL;

  if (info->IsImmutable())
    return NULL;

  TextureDefinition::LevelInfos level_infos(info->level_infos_.size());
  for (size_t face = 0; face < level_infos.size(); ++face) {
    GLenum target = info->target() == GL_TEXTURE_2D ?
        GL_TEXTURE_2D : FaceIndexToGLTarget(face);
    for (size_t level = 0; level < info->level_infos_[face].size(); ++level) {
      const TextureInfo::LevelInfo& level_info =
          info->level_infos_[face][level];
      level_infos[face].push_back(
          TextureDefinition::LevelInfo(target,
                                       level_info.internal_format,
                                       level_info.width,
                                       level_info.height,
                                       level_info.depth,
                                       level_info.border,
                                       level_info.format,
                                       level_info.type,
                                       level_info.cleared));

      SetLevelInfo(info,
                   target,
                   level,
                   GL_RGBA,
                   0,
                   0,
                   0,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   true);
    }
  }

  GLuint old_service_id = info->service_id();

  GLuint new_service_id = 0;
  glGenTextures(1, &new_service_id);
  info->SetServiceId(new_service_id);

  return new TextureDefinition(info->target(),
                               old_service_id,
                               level_infos);
}
