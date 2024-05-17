void ExtensionInstalledBubble::ShowInternal() {
  BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser_);

  views::View* reference_view = NULL;
  if (type_ == APP) {
    if (browser_view->IsTabStripVisible()) {
      AbstractTabStripView* tabstrip = browser_view->tabstrip();
      views::View* ntp_button = tabstrip->GetNewTabButton();
      if (ntp_button && ntp_button->IsDrawn()) {
        reference_view = ntp_button;
      } else {
        reference_view = tabstrip;
      }
    }
  } else if (type_ == BROWSER_ACTION) {
    BrowserActionsContainer* container =
        browser_view->GetToolbarView()->browser_actions();
    if (container->animating() &&
        animation_wait_retries_++ < kAnimationWaitMaxRetry) {
      MessageLoopForUI::current()->PostDelayedTask(
          FROM_HERE,
          base::Bind(&ExtensionInstalledBubble::ShowInternal,
                     base::Unretained(this)),
          kAnimationWaitTime);
      return;
    }
    reference_view = container->GetBrowserActionView(
        extension_->browser_action());
    if (!reference_view || !reference_view->visible()) {
      reference_view = container->chevron();
      if (!reference_view || !reference_view->visible())
        reference_view = NULL;   
    }
  } else if (type_ == PAGE_ACTION) {
    LocationBarView* location_bar_view = browser_view->GetLocationBarView();
    location_bar_view->SetPreviewEnabledPageAction(extension_->page_action(),
                                                   true);   
    reference_view = location_bar_view->GetPageActionView(
        extension_->page_action());
    DCHECK(reference_view);
  } else if (type_ == OMNIBOX_KEYWORD) {
    LocationBarView* location_bar_view = browser_view->GetLocationBarView();
    reference_view = location_bar_view;
    DCHECK(reference_view);
  }

  if (reference_view == NULL)
    reference_view = browser_view->GetToolbarView()->app_menu();
  set_anchor_view(reference_view);

  set_arrow_location(type_ == OMNIBOX_KEYWORD ? views::BubbleBorder::TOP_LEFT :
                                                views::BubbleBorder::TOP_RIGHT);
  SetLayoutManager(new views::FillLayout());
  AddChildView(
      new InstalledBubbleContent(browser_, extension_, type_, &icon_, this));
  browser::CreateViewsBubble(this);
  StartFade(true);
}
