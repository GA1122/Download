 int CompareFullHashes(const GURL& url,
                      const std::vector<SBFullHashResult>& full_hashes) {
  if (full_hashes.empty())
    return -1;

  std::vector<std::string> hosts, paths;
  GenerateHostsToCheck(url, &hosts);
  GeneratePathsToCheck(url, &paths);

  for (size_t h = 0; h < hosts.size(); ++h) {
    for (size_t p = 0; p < paths.size(); ++p) {
      SBFullHash key;
      base::SHA256HashString(hosts[h] + paths[p],
                             key.full_hash,
                             sizeof(SBFullHash));

      for (size_t i = 0; i < full_hashes.size(); ++i) {
        if (key == full_hashes[i].hash)
          return static_cast<int>(i);
      }
    }
  }

  return -1;
}
