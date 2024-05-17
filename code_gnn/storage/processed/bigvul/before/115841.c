NavigationEntry* NavigationController::CreateNavigationEntry(
    const GURL& url, const content::Referrer& referrer,
    content::PageTransition transition,
    bool is_renderer_initiated, const std::string& extra_headers,
    content::BrowserContext* browser_context) {
  GURL loaded_url(url);
  bool reverse_on_redirect = false;
  BrowserURLHandler::GetInstance()->RewriteURLIfNecessary(
      &loaded_url, browser_context, &reverse_on_redirect);

  NavigationEntry* entry = new NavigationEntry(
      NULL,   
      -1,
      loaded_url,
      referrer,
      string16(),
      transition,
      is_renderer_initiated);
  entry->set_virtual_url(url);
  entry->set_user_typed_url(url);
  entry->set_update_virtual_url_with_url(reverse_on_redirect);
  entry->set_extra_headers(extra_headers);
  return entry;
}
