bool RenderFrameHostImpl::CanCommitOrigin(
    const url::Origin& origin,
    const GURL& url) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableWebSecurity)) {
    return true;
  }

  if (origin.scheme() == url::kFileScheme) {
    WebPreferences prefs = render_view_host_->GetWebkitPreferences();
    if (prefs.allow_universal_access_from_file_urls)
      return true;
  }

  if (origin.unique())
    return true;

  if (url.IsStandard() &&
      !origin.IsSamePhysicalOriginWith(url::Origin::Create(url)))
    return false;

  GURL origin_url = origin.GetPhysicalOrigin().GetURL();

  return CanCommitURL(origin_url);
}