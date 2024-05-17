void BrowserView::Show() {
#if !defined(OS_WIN) && !defined(OS_CHROMEOS)
  BrowserList::SetLastActive(browser());
#endif

  if (frame_->IsVisible()) {
    frame_->Activate();
    return;
  }

  force_location_bar_focus_ = true;

  RestoreFocus();

  frame_->Show();

  force_location_bar_focus_ = false;

  browser()->OnWindowDidShow();

  MaybeShowInvertBubbleView(this);
}
