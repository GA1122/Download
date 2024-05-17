void InterstitialPageImpl::Show() {
  if (!enabled())
    return;

  InterstitialPageMap::const_iterator iter =
      g_web_contents_to_interstitial_page->find(web_contents_);
  if (iter != g_web_contents_to_interstitial_page->end()) {
    InterstitialPageImpl* interstitial = iter->second;
    if (interstitial->action_taken_ != NO_ACTION) {
      interstitial->Hide();
    } else {
      if (new_navigation_ && interstitial->new_navigation_)
        interstitial->should_discard_pending_nav_entry_= false;
      interstitial->DontProceed();
    }
  }

  TakeActionOnResourceDispatcher(BLOCK);
  notification_registrar_.Add(
      this, NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED,
      Source<RenderWidgetHost>(
          controller_->delegate()->GetRenderViewHost()->GetWidget()));

  iter = g_web_contents_to_interstitial_page->find(web_contents_);
  DCHECK(iter == g_web_contents_to_interstitial_page->end());
  (*g_web_contents_to_interstitial_page)[web_contents_] = this;

  if (new_navigation_) {
    std::unique_ptr<NavigationEntryImpl> entry =
        base::WrapUnique(new NavigationEntryImpl);
    entry->SetURL(url_);
    entry->SetVirtualURL(url_);
    entry->set_page_type(PAGE_TYPE_INTERSTITIAL);

    delegate_->OverrideEntry(entry.get());

    controller_->SetTransientEntry(std::move(entry));

    static_cast<WebContentsImpl*>(web_contents_)
        ->DidChangeVisibleSecurityState();
  }

  DCHECK(!render_view_host_);
  render_view_host_ = CreateRenderViewHost();
  CreateWebContentsView();

  GURL data_url = GURL("data:text/html;charset=utf-8," +
                       net::EscapePath(delegate_->GetHTMLContents()));
  frame_tree_->root()->current_frame_host()->NavigateToInterstitialURL(
      data_url);
  frame_tree_->root()->current_frame_host()->UpdateAccessibilityMode();

  notification_registrar_.Add(this, NOTIFICATION_NAV_ENTRY_PENDING,
      Source<NavigationController>(controller_));
}
