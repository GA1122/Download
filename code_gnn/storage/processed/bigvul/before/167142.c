void HTMLMediaElement::DidEnterFullscreen() {
  UpdateControlsVisibility();

  if (web_media_player_) {
    if (!LayoutTestSupport::IsRunningLayoutTest())
      web_media_player_->EnteredFullscreen();
    web_media_player_->OnDisplayTypeChanged(DisplayType());
  }

  in_overlay_fullscreen_video_ = UsesOverlayFullscreenVideo();
  if (in_overlay_fullscreen_video_) {
    GetDocument().GetLayoutView()->Compositor()->SetNeedsCompositingUpdate(
        kCompositingUpdateRebuildTree);
  }
}
