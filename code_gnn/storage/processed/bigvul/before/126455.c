void BrowserWindowGtk::UserChangedTheme() {
  SetBackgroundColor();
  InvalidateWindow();
  UpdateWindowShape(bounds_.width(), bounds_.height());
}
