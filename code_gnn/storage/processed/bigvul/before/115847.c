void NavigationController::LoadURLFromRenderer(
    const GURL& url,
    const content::Referrer& referrer,
    content::PageTransition transition,
    const std::string& extra_headers) {
  needs_reload_ = false;

  NavigationEntry* entry = CreateNavigationEntry(url, referrer, transition,
                                                 true,
                                                 extra_headers,
                                                 browser_context_);

  LoadEntry(entry);
}
