WebContentsImpl::CreateThrottlesForNavigation(
    NavigationHandle* navigation_handle) {
  return GetContentClient()->browser()->CreateThrottlesForNavigation(
      navigation_handle);
}
