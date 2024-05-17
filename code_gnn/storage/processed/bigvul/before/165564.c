void ContentSecurityPolicy::TreatAsPublicAddress() {
  if (!RuntimeEnabledFeatures::CorsRFC1918Enabled())
    return;
  treat_as_public_address_ = true;
}
