void BrowserView::ResetToolbarTabState(content::WebContents* contents) {
  if (toolbar_)
    toolbar_->ResetTabState(contents);
}
