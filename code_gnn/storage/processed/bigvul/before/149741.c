bool FeatureInfo::IsES3Capable() const {
  if (workarounds_.disable_texture_storage)
    return false;
  if (gl_version_info_)
    return gl_version_info_->is_es3_capable;
  return false;
}
