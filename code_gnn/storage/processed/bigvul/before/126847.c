void BrowserView::SetDownloadShelfVisible(bool visible) {
  if (browser_ == NULL)
    return;

  if (visible && IsDownloadShelfVisible() != visible) {
    GetDownloadShelf();
  }

  if (browser_ != NULL)
    browser_->UpdateDownloadShelfVisibility(visible);

  ToolbarSizeChanged(false);
}
