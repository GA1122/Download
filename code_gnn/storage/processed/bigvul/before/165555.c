void ContentSecurityPolicy::SetOverrideURLForSelf(const KURL& url) {
  scoped_refptr<const SecurityOrigin> origin = SecurityOrigin::Create(url);
  self_protocol_ = origin->Protocol();
  self_source_ =
      new CSPSource(this, self_protocol_, origin->Host(), origin->Port(),
                    String(), CSPSource::kNoWildcard, CSPSource::kNoWildcard);
}
