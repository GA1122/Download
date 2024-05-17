bool TextureManager::TextureInfo::CanRender(
    const FeatureInfo* feature_info) const {
  if (target_ == 0) {
    return false;
  }
  bool needs_mips = NeedsMips();
  if ((npot() && !feature_info->feature_flags().npot_ok) ||
      (target_ == GL_TEXTURE_RECTANGLE_ARB)) {
    return !needs_mips &&
           wrap_s_ == GL_CLAMP_TO_EDGE &&
           wrap_t_ == GL_CLAMP_TO_EDGE;
  }
  if (needs_mips) {
    if (target_ == GL_TEXTURE_2D) {
      return texture_complete();
    } else {
      return texture_complete() && cube_complete();
    }
  } else {
    return true;
  }
}
