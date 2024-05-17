void BrowserView::FullscreenStateChanged() {
  bool fullscreen = IsFullscreen();
  ProcessFullscreen(
      fullscreen, GURL(),
      fullscreen
          ? GetExclusiveAccessManager()->GetExclusiveAccessExitBubbleType()
          : EXCLUSIVE_ACCESS_BUBBLE_TYPE_NONE);
  frame_->GetFrameView()->OnFullscreenStateChanged();
}
