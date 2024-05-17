std::string TopSitesImpl::GetURLHash(const GURL& url) {
  return base::MD5String(url.spec());
}
