void TextureManager::SetLevelInfo(
    TextureManager::TextureInfo* info,
    GLenum target,
    GLint level,
    GLenum internal_format,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLint border,
    GLenum format,
    GLenum type,
    bool cleared) {
  DCHECK(info);
  if (!info->CanRender(feature_info_)) {
    DCHECK_NE(0, num_unrenderable_textures_);
    --num_unrenderable_textures_;
  }
  if (!info->SafeToRenderFrom()) {
    DCHECK_NE(0, num_unsafe_textures_);
    --num_unsafe_textures_;
  }
  num_uncleared_mips_ -= info->num_uncleared_mips();
  DCHECK_GE(num_uncleared_mips_, 0);
  mem_represented_ -= info->estimated_size();
  info->SetLevelInfo(
      feature_info_, target, level, internal_format, width, height, depth,
      border, format, type, cleared);
  mem_represented_ += info->estimated_size();
  UpdateMemRepresented();

  num_uncleared_mips_ += info->num_uncleared_mips();
  if (!info->CanRender(feature_info_)) {
    ++num_unrenderable_textures_;
  }
  if (!info->SafeToRenderFrom()) {
    ++num_unsafe_textures_;
  }
}
