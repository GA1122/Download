bool WebContentsImpl::WasRecentlyAudible() {
  return audio_stream_monitor_.WasRecentlyAudible() ||
         (browser_plugin_embedder_ &&
          browser_plugin_embedder_->WereAnyGuestsRecentlyAudible());
}
