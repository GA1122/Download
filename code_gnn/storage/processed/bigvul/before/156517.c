  bool CanAccessDataForOrigin(const GURL& site_url) {
    if (origin_lock_.is_empty())
      return true;
    return origin_lock_ == site_url;
  }
