  bool CanRequestOrigin(const url::Origin& origin) {
    return origin_map_.find(origin) != origin_map_.end();
  }
