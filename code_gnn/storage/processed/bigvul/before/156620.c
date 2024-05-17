void NavigatorImpl::DidStartMainFrameNavigation(
    const GURL& url,
    SiteInstanceImpl* site_instance,
    NavigationHandleImpl* navigation_handle) {
  NavigationEntryImpl* pending_entry = controller_->GetPendingEntry();
  bool has_browser_initiated_pending_entry =
      pending_entry && !pending_entry->is_renderer_initiated();

  bool renderer_provisional_load_to_pending_url =
      pending_entry && pending_entry->is_renderer_initiated() &&
      (pending_entry->GetURL() == url);

  bool has_transient_entry = !!controller_->GetTransientEntry();

  if (!has_browser_initiated_pending_entry && !has_transient_entry &&
      !renderer_provisional_load_to_pending_url) {
    std::unique_ptr<NavigationEntryImpl> entry =
        NavigationEntryImpl::FromNavigationEntry(
            NavigationController::CreateNavigationEntry(
                url, content::Referrer(), ui::PAGE_TRANSITION_LINK,
                true  , std::string(),
                controller_->GetBrowserContext(),
                nullptr  ));
    if (pending_entry) {
      entry->set_should_replace_entry(pending_entry->should_replace_entry());
      entry->SetRedirectChain(pending_entry->GetRedirectChain());
    }

    controller_->SetPendingEntry(std::move(entry));
    if (delegate_)
      delegate_->NotifyChangedNavigationState(content::INVALIDATE_TYPE_URL);
  }
}
