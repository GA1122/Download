void WebMediaPlayerImpl::ExitedFullscreen() {
  overlay_info_.is_fullscreen = false;

  if (!always_enable_overlays_ && overlay_enabled_)
    DisableOverlay();

  if (!decoder_requires_restart_for_overlay_)
    MaybeSendOverlayInfoToDecoder();
}
