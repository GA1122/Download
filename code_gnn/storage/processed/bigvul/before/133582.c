WebPreferences WebContentsImpl::GetWebkitPrefs() {
  GURL url = controller_.GetActiveEntry()
      ? controller_.GetActiveEntry()->GetURL() : GURL::EmptyGURL();

  return GetRenderManager()->current_host()->GetWebkitPrefs(url);
}
