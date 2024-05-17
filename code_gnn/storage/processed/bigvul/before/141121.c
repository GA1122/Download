void Document::InitDNSPrefetch() {
  Settings* settings = GetSettings();

  have_explicitly_disabled_dns_prefetch_ = false;
  is_dns_prefetch_enabled_ = settings && settings->GetDNSPrefetchingEnabled() &&
                             GetSecurityOrigin()->Protocol() == "http";

  if (Document* parent = ParentDocument()) {
    if (!parent->IsDNSPrefetchEnabled())
      is_dns_prefetch_enabled_ = false;
  }
}
