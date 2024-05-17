bool WebContentsImpl::WasRecentlyAudible() {
  return audio_stream_monitor_.WasRecentlyAudible();
}
