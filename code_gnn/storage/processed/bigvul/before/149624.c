bool ResourcePrefetchPredictor::GetRedirectOrigin(
    const url::Origin& entry_origin,
    const RedirectDataMap& redirect_data,
    url::Origin* redirect_origin) {
  DCHECK(redirect_origin);

  RedirectData data;
  bool exists = redirect_data.TryGetData(entry_origin.host(), &data);
  if (!exists) {
    *redirect_origin = entry_origin;
    return true;
  }

  DCHECK_GT(data.redirect_endpoints_size(), 0);
  if (data.redirect_endpoints_size() > 1) {
    return false;
  }

  const float kMinRedirectConfidenceToTriggerPrefetch = 0.9f;
  const int kMinRedirectHitsToTriggerPrefetch = 2;

  const RedirectStat& redirect = data.redirect_endpoints(0);
  bool redirect_origin_matches_entry_origin =
      redirect.url() == entry_origin.host() &&
      redirect.url_port() == entry_origin.port();

  if (ComputeRedirectConfidence(redirect) <
          kMinRedirectConfidenceToTriggerPrefetch ||
      (redirect.number_of_hits() < kMinRedirectHitsToTriggerPrefetch &&
       !redirect_origin_matches_entry_origin)) {
    return false;
  }


  GURL redirect_url;
  if (redirect.has_url_scheme() && redirect.has_url_port()) {
    redirect_url = CreateRedirectURL(redirect.url_scheme(), redirect.url(),
                                     redirect.url_port());
  }

  if (!redirect_url.is_valid())
    redirect_url = CreateRedirectURL("https", redirect.url(), 443);

  if (!redirect_url.is_valid())
    return false;

  *redirect_origin = url::Origin::Create(redirect_url);
  return true;
}
