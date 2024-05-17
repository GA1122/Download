void HTMLMediaElement::didExitFullscreen() {
  updateControlsVisibility();

  if (webMediaPlayer())
    webMediaPlayer()->exitedFullscreen();
  if (m_inOverlayFullscreenVideo)
    document().layoutViewItem().compositor()->setNeedsCompositingUpdate(
        CompositingUpdateRebuildTree);
  m_inOverlayFullscreenVideo = false;
}
