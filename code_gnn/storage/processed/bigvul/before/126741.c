void BrowserView::ExitFullscreen() {
  if (!IsFullscreen())
    return;   

  ProcessFullscreen(false, FOR_DESKTOP, GURL(), FEB_TYPE_NONE);
}
