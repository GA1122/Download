bool WebContentsImpl::ShouldAllowRunningInsecureContent(
    WebContents* web_contents,
    bool allowed_per_prefs,
    const url::Origin& origin,
    const GURL& resource_url) {
  return GetDelegate()->ShouldAllowRunningInsecureContent(
      web_contents, allowed_per_prefs, origin, resource_url);
}
