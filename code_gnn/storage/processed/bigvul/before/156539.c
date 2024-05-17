  void GrantCommitOrigin(const url::Origin& origin) {
    if (origin.unique())
      return;
    origin_map_[origin] = CommitRequestPolicy::kCommitAndRequest;
  }
