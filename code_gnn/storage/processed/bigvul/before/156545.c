  void GrantRequestOrigin(const url::Origin& origin) {
    if (origin.unique())
      return;
    origin_map_.emplace(origin, CommitRequestPolicy::kRequestOnly);
  }
