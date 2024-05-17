void BrowserView::FullScreenStateChanged() {
  if (IsFullscreen()) {
    if (fullscreen_request_.pending) {
      fullscreen_request_.pending = false;
      ProcessFullscreen(true, FOR_DESKTOP,
                        fullscreen_request_.url,
                        fullscreen_request_.bubble_type);
    } else {
      ProcessFullscreen(true, FOR_DESKTOP, GURL(),
                        FEB_TYPE_BROWSER_FULLSCREEN_EXIT_INSTRUCTION);
    }
  } else {
    ProcessFullscreen(false, FOR_DESKTOP, GURL(), FEB_TYPE_NONE);
  }
}
