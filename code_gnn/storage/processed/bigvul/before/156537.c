bool ChildProcessSecurityPolicyImpl::GetMatchingIsolatedOrigin(
    const url::Origin& origin,
    const GURL& site_url,
    url::Origin* result) {
  *result = url::Origin();
  base::AutoLock lock(lock_);

  auto it = isolated_origins_.find(site_url);

  if (it == isolated_origins_.end() && site_url.has_host() &&
      site_url.host_piece().back() == '.') {
    GURL::Replacements replacements;
    base::StringPiece host(site_url.host_piece());
    host.remove_suffix(1);
    replacements.SetHostStr(host);
    it = isolated_origins_.find(site_url.ReplaceComponents(replacements));
  }

  bool found = false;
  if (it != isolated_origins_.end()) {
    for (const url::Origin& isolated_origin : it->second) {
      if (IsolatedOriginUtil::DoesOriginMatchIsolatedOrigin(origin,
                                                            isolated_origin)) {
        if (!found ||
            result->host().length() < isolated_origin.host().length()) {
          *result = isolated_origin;
          found = true;
        }
      }
    }
  }

  return found;
}
