void BrowserView::ExitFullscreen() {
  if (!IsFullscreen())
    return;   

  ProcessFullscreen(false, GURL(), EXCLUSIVE_ACCESS_BUBBLE_TYPE_NONE);
}
