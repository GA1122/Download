void TextureManager::StopTracking(TextureManager::TextureInfo* texture) {
  --texture_info_count_;
  if (!texture->CanRender(feature_info_)) {
    DCHECK_NE(0, num_unrenderable_textures_);
    --num_unrenderable_textures_;
  }
  if (!texture->SafeToRenderFrom()) {
    DCHECK_NE(0, num_unsafe_textures_);
    --num_unsafe_textures_;
  }
  num_uncleared_mips_ -= texture->num_uncleared_mips();
  DCHECK_GE(num_uncleared_mips_, 0);
  mem_represented_ -= texture->estimated_size();
  UpdateMemRepresented();
}
