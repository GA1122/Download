void RenderViewImpl::OnAudioStateChanged(bool is_audio_playing) {
  webview()->AudioStateChanged(is_audio_playing);
}
