bool TextureManager::ClearTextureLevel(
    GLES2Decoder* decoder,TextureManager::TextureInfo* info,
    GLenum target, GLint level) {
  DCHECK(info);
  if (info->num_uncleared_mips() == 0) {
    return true;
  }
  num_uncleared_mips_ -= info->num_uncleared_mips();
  DCHECK_GE(num_uncleared_mips_, 0);
  if (!info->SafeToRenderFrom()) {
    DCHECK_NE(0, num_unsafe_textures_);
    --num_unsafe_textures_;
  }
  bool result = info->ClearLevel(decoder, target, level);
  info->UpdateCleared();
  num_uncleared_mips_ += info->num_uncleared_mips();
  if (!info->SafeToRenderFrom()) {
    ++num_unsafe_textures_;
  }
  return result;
}
