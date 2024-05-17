void BrowserView::UpdateToolbar(content::WebContents* contents) {
  if (toolbar_)
    toolbar_->Update(contents);
}
