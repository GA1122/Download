void TextureManager::TextureInfo::SetLevelCleared(GLenum target, GLint level) {
  DCHECK_GE(level, 0);
  DCHECK_LT(static_cast<size_t>(GLTargetToFaceIndex(target)),
            level_infos_.size());
  DCHECK_LT(static_cast<size_t>(level),
            level_infos_[GLTargetToFaceIndex(target)].size());
  TextureInfo::LevelInfo& info =
      level_infos_[GLTargetToFaceIndex(target)][level];
  if (!info.cleared) {
    DCHECK_NE(0, num_uncleared_mips_);
    --num_uncleared_mips_;
  }
  info.cleared = true;
  UpdateCleared();
}
