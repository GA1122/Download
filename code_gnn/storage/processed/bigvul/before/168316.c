void BrowserView::Layout() {
  if (!initialized_ || in_process_fullscreen_)
    return;

  views::View::Layout();

  toolbar_->location_bar()->omnibox_view()->SetFocusBehavior(
      IsToolbarVisible() ? FocusBehavior::ALWAYS : FocusBehavior::NEVER);
  frame()->GetFrameView()->UpdateMinimumSize();
}
