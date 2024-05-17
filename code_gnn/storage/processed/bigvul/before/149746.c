bool FeatureInfo::IsWebGL2OrES3OrHigherContext() const {
  return IsWebGL2OrES3OrHigherContextType(context_type_);
}
