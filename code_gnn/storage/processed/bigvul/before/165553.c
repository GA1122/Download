void ContentSecurityPolicy::RequireTrustedTypes() {
  if (!RuntimeEnabledFeatures::TrustedDOMTypesEnabled())
    return;
  require_safe_types_ = true;
}
