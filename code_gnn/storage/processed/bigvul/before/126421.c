void BrowserWindowGtk::SetFocusToLocationBar(bool select_all) {
  if (!IsFullscreen())
    GetLocationBar()->FocusLocation(select_all);
}
