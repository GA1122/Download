void BrowserView::TabDeactivated(WebContents* contents) {
  if (!contents->IsBeingDestroyed())
    contents->StoreFocus();
}
