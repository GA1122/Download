HostCache::Key Key(const std::string& hostname) {
  return HostCache::Key(hostname, ADDRESS_FAMILY_UNSPECIFIED, 0);
}
