bool TextureManager::ClearRenderableLevels(
    GLES2Decoder* decoder,TextureManager::TextureInfo* info) {
  DCHECK(info);
  if (info->SafeToRenderFrom()) {
    return true;
  }
  DCHECK_NE(0, num_unsafe_textures_);
  --num_unsafe_textures_;
  num_uncleared_mips_ -= info->num_uncleared_mips();
  DCHECK_GE(num_uncleared_mips_, 0);
  bool result = info->ClearRenderableLevels(decoder);
  num_uncleared_mips_ += info->num_uncleared_mips();
  if (!info->SafeToRenderFrom()) {
    ++num_unsafe_textures_;
  }
  return result;
}
