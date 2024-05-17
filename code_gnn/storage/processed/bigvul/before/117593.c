void SettingLevelBubbleView::Layout() {
  progress_bar_->SetBounds(width() - kPadding - kProgressBarWidth,
                           (height() - kProgressBarHeight) / 2,
                           kProgressBarWidth, kProgressBarHeight);
}
