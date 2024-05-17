bool WebMediaPlayerImpl::SupportsOverlayFullscreenVideo() {
#if defined(OS_ANDROID)
  return !using_media_player_renderer_ &&
         overlay_mode_ == OverlayMode::kUseContentVideoView;
#else
  return false;
#endif
}
