void HTMLMediaElement::DidExitFullscreen() {
  UpdateControlsVisibility();

  if (GetWebMediaPlayer()) {
    GetWebMediaPlayer()->ExitedFullscreen();
    GetWebMediaPlayer()->OnDisplayTypeChanged(DisplayType());
  }

  if (in_overlay_fullscreen_video_) {
    GetDocument().GetLayoutView()->Compositor()->SetNeedsCompositingUpdate(
        kCompositingUpdateRebuildTree);
  }
  in_overlay_fullscreen_video_ = false;
}
