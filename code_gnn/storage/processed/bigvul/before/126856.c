void BrowserView::Show() {
  if (frame_->IsVisible()) {
    frame_->Activate();
    return;
  }

  CreateLauncherIcon();

  force_location_bar_focus_ = true;

  RestoreFocus();

  frame_->Show();

  force_location_bar_focus_ = false;

  browser()->OnWindowDidShow();

  chrome::MaybeShowInvertBubbleView(browser_.get(), contents_);
}
