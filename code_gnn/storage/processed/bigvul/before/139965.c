void HTMLMediaElement::didEnterFullscreen() {
  updateControlsVisibility();

  if (webMediaPlayer() && !LayoutTestSupport::isRunningLayoutTest())
    webMediaPlayer()->enteredFullscreen();
  m_inOverlayFullscreenVideo = usesOverlayFullscreenVideo();
  if (m_inOverlayFullscreenVideo)
    document().layoutViewItem().compositor()->setNeedsCompositingUpdate(
        CompositingUpdateRebuildTree);
}
