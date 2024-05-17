bool ContentSecurityPolicy::UrlMatchesSelf(const KURL& url) const {
  return self_source_->MatchesAsSelf(url);
}
