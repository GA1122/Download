bool IsGoogleSearchUrl(const GURL& url) {
  if (!IsGoogleDomainUrl(url, DISALLOW_SUBDOMAIN,
                         DISALLOW_NON_STANDARD_PORTS) &&
      !IsGoogleSearchSubdomainUrl(url)) {
    return false;
  }

  base::StringPiece path(url.path_piece());
  bool is_home_page_base = IsPathHomePageBase(path);
  if (!is_home_page_base && (path != "/search"))
    return false;

  return HasGoogleSearchQueryParam(url.ref_piece()) ||
         (!is_home_page_base && HasGoogleSearchQueryParam(url.query_piece()));
}
