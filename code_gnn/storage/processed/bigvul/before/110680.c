bool TextureManager::Restore(TextureInfo* info,
                             TextureDefinition* definition) {
  DCHECK(info->owned_);

  scoped_ptr<TextureDefinition> scoped_definition(definition);

  if (info->IsAttachedToFramebuffer())
    return false;

  if (info->IsImmutable())
    return false;

  if (info->target() != definition->target())
    return false;

  if (info->level_infos_.size() != definition->level_infos().size())
    return false;

  if (info->level_infos_[0].size() != definition->level_infos()[0].size())
    return false;

  for (size_t face = 0; face < info->level_infos_.size(); ++face) {
    GLenum target = info->target() == GL_TEXTURE_2D ?
        GL_TEXTURE_2D : FaceIndexToGLTarget(face);
    for (size_t level = 0; level < info->level_infos_[face].size(); ++level) {
      const TextureDefinition::LevelInfo& level_info =
          definition->level_infos()[face][level];
      SetLevelInfo(info,
                   target,
                   level,
                   level_info.internal_format,
                   level_info.width,
                   level_info.height,
                   level_info.depth,
                   level_info.border,
                   level_info.format,
                   level_info.type,
                   level_info.cleared);
    }
  }

  GLuint old_service_id = info->service_id();
  glDeleteTextures(1, &old_service_id);
  info->SetServiceId(definition->ReleaseServiceId());

  return true;
}
