bool CheckETCFormatSupport(const FeatureInfo& feature_info) {
  const gl::GLVersionInfo& version_info = feature_info.gl_version_info();
  return version_info.IsAtLeastGL(4, 3) || version_info.IsAtLeastGLES(3, 0) ||
         feature_info.feature_flags().arb_es3_compatibility;
}
