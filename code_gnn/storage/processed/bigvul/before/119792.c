bool NavigationControllerImpl::IsURLInPageNavigation(
    const GURL& url,
    bool renderer_says_in_page,
    NavigationType navigation_type) const {
  NavigationEntry* last_committed = GetLastCommittedEntry();
  return last_committed && AreURLsInPageNavigation(
      last_committed->GetURL(), url, renderer_says_in_page, navigation_type);
}
