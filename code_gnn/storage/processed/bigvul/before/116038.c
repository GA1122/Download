void ExtensionInstalledBubble::WindowClosing() {
  if (extension_ && type_ == PAGE_ACTION) {
    BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser_);
    browser_view->GetLocationBarView()->SetPreviewEnabledPageAction(
        extension_->page_action(),
        false);   
  }
}
