void BrowserView::UpdateUIForTabFullscreen(TabFullscreenState state) {
  frame()->GetFrameView()->UpdateFullscreenTopUI(
      state == ExclusiveAccessContext::STATE_EXIT_TAB_FULLSCREEN);
}
