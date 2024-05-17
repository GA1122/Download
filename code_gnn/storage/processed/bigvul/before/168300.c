void BrowserView::HideDownloadShelf() {
  if (download_shelf_)
    download_shelf_->Hide();

  StatusBubble* status_bubble = GetStatusBubble();
  if (status_bubble)
    status_bubble->Hide();
}
