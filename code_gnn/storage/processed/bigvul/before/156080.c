bool IsGoogleHomePageUrl(const GURL& url) {
  if (!IsGoogleDomainUrl(url, DISALLOW_SUBDOMAIN,
                         DISALLOW_NON_STANDARD_PORTS) &&
      !IsGoogleSearchSubdomainUrl(url)) {
    return false;
  }

  base::StringPiece path(url.path_piece());
  return IsPathHomePageBase(path) ||
         base::StartsWith(path, "/ig", base::CompareCase::INSENSITIVE_ASCII);
}
