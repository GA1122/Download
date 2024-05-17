std::unique_ptr<NavigationUIData> WebContentsImpl::GetNavigationUIData(
    NavigationHandle* navigation_handle) {
  DCHECK(IsBrowserSideNavigationEnabled());
  return GetContentClient()->browser()->GetNavigationUIData(navigation_handle);
}
