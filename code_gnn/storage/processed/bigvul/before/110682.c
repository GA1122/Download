void TextureManager::SetInfoTarget(
    TextureManager::TextureInfo* info, GLenum target) {
  DCHECK(info);
  if (!info->CanRender(feature_info_)) {
    DCHECK_NE(0, num_unrenderable_textures_);
    --num_unrenderable_textures_;
  }
  info->SetTarget(target, MaxLevelsForTarget(target));
  if (!info->CanRender(feature_info_)) {
    ++num_unrenderable_textures_;
  }
}
