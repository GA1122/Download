bool ContentSecurityPolicy::experimentalFeaturesEnabled() const {
  return RuntimeEnabledFeatures::
      experimentalContentSecurityPolicyFeaturesEnabled();
}
