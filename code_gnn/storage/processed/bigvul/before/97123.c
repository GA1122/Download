bool NavigationController::IsURLInPageNavigation(const GURL& url) const {
  NavigationEntry* last_committed = GetLastCommittedEntry();
  if (!last_committed)
    return false;
  return AreURLsInPageNavigation(last_committed->url(), url);
}
