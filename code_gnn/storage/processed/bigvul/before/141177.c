void Document::ParseDNSPrefetchControlHeader(
    const String& dns_prefetch_control) {
  if (DeprecatedEqualIgnoringCase(dns_prefetch_control, "on") &&
      !have_explicitly_disabled_dns_prefetch_) {
    is_dns_prefetch_enabled_ = true;
    return;
  }

  is_dns_prefetch_enabled_ = false;
  have_explicitly_disabled_dns_prefetch_ = true;
}
