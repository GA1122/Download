void WebMediaPlayerImpl::SetIsEffectivelyFullscreen(
    blink::WebFullscreenVideoStatus fullscreen_video_status) {
  delegate_->SetIsEffectivelyFullscreen(delegate_id_, fullscreen_video_status);
}
