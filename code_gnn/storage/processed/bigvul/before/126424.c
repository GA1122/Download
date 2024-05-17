bool BrowserWindowGtk::ShouldDrawContentDropShadow() const {
  return !IsMaximized() && UseCustomFrame();
}
