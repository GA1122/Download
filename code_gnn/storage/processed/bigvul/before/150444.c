std::unique_ptr<NavigationEntry> NavigationController::CreateNavigationEntry(
    const GURL& url,
    const Referrer& referrer,
    ui::PageTransition transition,
    bool is_renderer_initiated,
    const std::string& extra_headers,
    BrowserContext* browser_context) {
  GURL dest_url(url);
  BrowserURLHandlerImpl::GetInstance()->FixupURLBeforeRewrite(&dest_url,
                                                              browser_context);

  GURL loaded_url(dest_url);
  bool reverse_on_redirect = false;
  BrowserURLHandlerImpl::GetInstance()->RewriteURLIfNecessary(
      &loaded_url, browser_context, &reverse_on_redirect);

  NavigationEntryImpl* entry = new NavigationEntryImpl(
      NULL,   
      loaded_url,
      referrer,
      base::string16(),
      transition,
      is_renderer_initiated);
  entry->SetVirtualURL(dest_url);
  entry->set_user_typed_url(dest_url);
  entry->set_update_virtual_url_with_url(reverse_on_redirect);
  entry->set_extra_headers(extra_headers);
  return base::WrapUnique(entry);
}
