  void GrantCommitScheme(const std::string& scheme) {
    scheme_map_[scheme] = CommitRequestPolicy::kCommitAndRequest;
  }
