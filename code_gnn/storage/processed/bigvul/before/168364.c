void BrowserView::ShowInactive() {
  if (!frame_->IsVisible())
    frame_->ShowInactive();
}
