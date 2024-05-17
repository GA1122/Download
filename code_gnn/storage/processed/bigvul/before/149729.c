void FeatureInfo::EnableEXTFloatBlend() {
  if (!feature_flags_.ext_float_blend) {
    AddExtensionString("GL_EXT_float_blend");
    feature_flags_.ext_float_blend = true;
  }
}
