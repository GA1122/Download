void BrowserView::ShowInactive() {
  if (frame_->IsVisible())
    return;
  CreateLauncherIcon();
  frame_->ShowInactive();
}
