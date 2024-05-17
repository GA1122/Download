void NavigationController::TransferURL(
    const GURL& url,
    const content::Referrer& referrer,
    content::PageTransition transition,
    const std::string& extra_headers,
    const GlobalRequestID& transferred_global_request_id,
    bool is_renderer_initiated) {
  needs_reload_ = false;

  NavigationEntry* entry = CreateNavigationEntry(url, referrer, transition,
                                                 is_renderer_initiated,
                                                 extra_headers,
                                                 browser_context_);
  entry->set_transferred_global_request_id(transferred_global_request_id);

  LoadEntry(entry);
}
