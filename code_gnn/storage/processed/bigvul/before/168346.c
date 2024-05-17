void BrowserView::SaveWindowPlacement(const gfx::Rect& bounds,
                                      ui::WindowShowState show_state) {
  if (!IsFullscreen() && frame_->ShouldSaveWindowPlacement() &&
      chrome::ShouldSaveWindowPlacement(browser_.get())) {
    WidgetDelegate::SaveWindowPlacement(bounds, show_state);
    chrome::SaveWindowPlacement(browser_.get(), bounds, show_state);
  }
}
