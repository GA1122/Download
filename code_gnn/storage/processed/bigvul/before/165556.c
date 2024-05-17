void ContentSecurityPolicy::SetupSelf(const SecurityOrigin& security_origin) {
  self_protocol_ = security_origin.Protocol();
  self_source_ = new CSPSource(this, self_protocol_, security_origin.Host(),
                               security_origin.Port(), String(),
                               CSPSource::kNoWildcard, CSPSource::kNoWildcard);
}
