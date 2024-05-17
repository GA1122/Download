void ContentSecurityPolicy::treatAsPublicAddress() {
  if (!RuntimeEnabledFeatures::corsRFC1918Enabled())
    return;
  m_treatAsPublicAddress = true;
}
