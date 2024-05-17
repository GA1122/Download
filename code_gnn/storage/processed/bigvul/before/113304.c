bool NativePanelTestingWin::IsButtonVisible(
    TitlebarButtonType button_type) const {
  PanelBrowserFrameView* frame_view = panel_browser_view_->GetFrameView();

  switch (button_type) {
    case CLOSE_BUTTON:
      return frame_view->close_button_->visible();
    case MINIMIZE_BUTTON:
      return frame_view->minimize_button_->visible();
    case RESTORE_BUTTON:
      return frame_view->restore_button_->visible();
    default:
      NOTREACHED();
  }
  return false;
}
