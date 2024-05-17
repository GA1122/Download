bool TextureManager::TextureInfo::ClearRenderableLevels(GLES2Decoder* decoder) {
  DCHECK(decoder);
  if (SafeToRenderFrom()) {
    return true;
  }

  const TextureInfo::LevelInfo& first_face = level_infos_[0][0];
  int levels_needed = ComputeMipMapCount(
      first_face.width, first_face.height, first_face.depth);

  for (size_t ii = 0; ii < level_infos_.size(); ++ii) {
    for (GLint jj = 0; jj < levels_needed; ++jj) {
      TextureInfo::LevelInfo& info = level_infos_[ii][jj];
      if (info.target != 0) {
        if (!ClearLevel(decoder, info.target, jj)) {
          return false;
        }
      }
    }
  }
  cleared_ = true;
  return true;
}
