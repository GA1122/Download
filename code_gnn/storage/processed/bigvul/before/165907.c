NavigationDownloadPolicy GetDownloadPolicy(
    bool prevent_sandboxed_download,
    bool is_opener_navigation,
    const blink::WebURLRequest& request,
    const WebSecurityOrigin& current_origin) {
  if (prevent_sandboxed_download)
    return NavigationDownloadPolicy::kDisallowSandbox;
  if (!is_opener_navigation)
    return NavigationDownloadPolicy::kAllow;
  bool gesture = request.HasUserGesture();
  bool cross_origin = !request.RequestorOrigin().CanAccess(current_origin);
  if (!gesture && cross_origin)
    return NavigationDownloadPolicy::kAllowOpenerCrossOriginNoGesture;
  if (!gesture)
    return NavigationDownloadPolicy::kAllowOpenerNoGesture;
  if (cross_origin)
    return NavigationDownloadPolicy::kAllowOpenerCrossOrigin;
  return NavigationDownloadPolicy::kAllowOpener;
}
