TopDomainEntry LookupSkeletonInTopDomains(const std::string& skeleton) {
  return g_idn_spoof_checker.Get().LookupSkeletonInTopDomains(skeleton);
}
