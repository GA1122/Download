bool TextureManager::TextureInfo::ClearLevel(
    GLES2Decoder* decoder, GLenum target, GLint level) {
  DCHECK(decoder);
  size_t face_index = GLTargetToFaceIndex(target);
  if (face_index >= level_infos_.size() ||
      level >= static_cast<GLint>(level_infos_[face_index].size())) {
    return true;
  }

  TextureInfo::LevelInfo& info = level_infos_[face_index][level];

  DCHECK(target == info.target);

  if (info.target == 0 ||
      info.cleared ||
      info.width == 0 ||
      info.height == 0 ||
      info.depth == 0) {
    return true;
  }

  DCHECK_NE(0, num_uncleared_mips_);
  --num_uncleared_mips_;

  info.cleared = decoder->ClearLevel(
      service_id_, target_, info.target, info.level, info.format, info.type,
      info.width, info.height, immutable_);
  if (!info.cleared) {
    ++num_uncleared_mips_;
  }
  return info.cleared;
}
