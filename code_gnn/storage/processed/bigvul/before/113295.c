void PanelBrowserView::FullScreenModeChanged(bool is_full_screen) {
  if (is_full_screen) {
    if (frame()->IsVisible()) {
        frame()->Hide();
    }
  } else {
    ShowInactive();
  }
}
