void WebContents::SetScreenOrientationDelegate(
    ScreenOrientationDelegate* delegate) {
  ScreenOrientationProvider::SetDelegate(delegate);
}
