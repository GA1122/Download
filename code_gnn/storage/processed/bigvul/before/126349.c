int BrowserWindowGtk::GetExtraRenderViewHeight() const {
  int sum = infobar_container_->TotalHeightOfAnimatingBars();
  if (IsBookmarkBarSupported() && bookmark_bar_->IsAnimating())
    sum += bookmark_bar_->GetHeight();
  if (download_shelf_.get() && download_shelf_->IsClosing())
    sum += download_shelf_->GetHeight();
  return sum;
}
