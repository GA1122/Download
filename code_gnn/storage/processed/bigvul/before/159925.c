  GURL GetCanonicalURL(const GURL& url) {
    return top_sites()->cache_->GetCanonicalURL(url);
  }
