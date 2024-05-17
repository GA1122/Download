void LoadingPredictor::HandleOmniboxHint(const GURL& url, bool preconnectable) {
  if (!url.is_valid() || !url.has_host() || !IsPreconnectAllowed(profile_))
    return;

  GURL origin = url.GetOrigin();
  bool is_new_origin = origin != last_omnibox_origin_;
  last_omnibox_origin_ = origin;
  base::TimeTicks now = base::TimeTicks::Now();
  if (preconnectable) {
    if (is_new_origin || now - last_omnibox_preconnect_time_ >=
                             kMinDelayBetweenPreconnectRequests) {
      last_omnibox_preconnect_time_ = now;
      url::Origin url_origin = url::Origin::Create(url);
      preconnect_manager()->StartPreconnectUrl(
          url, true, net::NetworkIsolationKey(url_origin, url_origin));
    }
    return;
  }

  if (is_new_origin || now - last_omnibox_preresolve_time_ >=
                           kMinDelayBetweenPreresolveRequests) {
    last_omnibox_preresolve_time_ = now;
    preconnect_manager()->StartPreresolveHost(url);
  }
}
