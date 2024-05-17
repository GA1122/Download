bool WebContentsImpl::IsCurrentlyAudible() {
  return audio_stream_monitor()->IsCurrentlyAudible();
}
