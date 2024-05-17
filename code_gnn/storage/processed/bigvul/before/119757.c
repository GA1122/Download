NavigationEntry* NavigationController::CreateNavigationEntry(
      const GURL& url,
      const Referrer& referrer,
      PageTransition transition,
      bool is_renderer_initiated,
      const std::string& extra_headers,
      BrowserContext* browser_context) {
  GURL loaded_url(url);
  bool reverse_on_redirect = false;
  BrowserURLHandlerImpl::GetInstance()->RewriteURLIfNecessary(
      &loaded_url, browser_context, &reverse_on_redirect);

  NavigationEntryImpl* entry = new NavigationEntryImpl(
      NULL,   
      -1,
      loaded_url,
      referrer,
      string16(),
      transition,
      is_renderer_initiated);
  entry->SetVirtualURL(url);
  entry->set_user_typed_url(url);
  entry->set_update_virtual_url_with_url(reverse_on_redirect);
  entry->set_extra_headers(extra_headers);
  return entry;
}
