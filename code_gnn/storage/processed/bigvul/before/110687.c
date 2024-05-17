bool TextureManager::SetParameter(
    TextureManager::TextureInfo* info, GLenum pname, GLint param) {
  DCHECK(info);
  if (!info->CanRender(feature_info_)) {
    DCHECK_NE(0, num_unrenderable_textures_);
    --num_unrenderable_textures_;
  }
  if (!info->SafeToRenderFrom()) {
    DCHECK_NE(0, num_unsafe_textures_);
    --num_unsafe_textures_;
  }
  bool result = info->SetParameter(feature_info_, pname, param);
  if (!info->CanRender(feature_info_)) {
    ++num_unrenderable_textures_;
  }
  if (!info->SafeToRenderFrom()) {
    ++num_unsafe_textures_;
  }
  return result;
}
