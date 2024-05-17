ChromeContentBrowserClient::GetAdditionalSiteIsolationModes() {
  if (SiteIsolationPolicy::IsIsolationForPasswordSitesEnabled())
    return {"Isolate Password Sites"};
  else
    return {};
}
