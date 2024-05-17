void WebContentsImpl::OnAudioStateChanged() {
  bool is_currently_audible =
      audio_stream_monitor_.IsCurrentlyAudible() ||
      (browser_plugin_embedder_ &&
       browser_plugin_embedder_->AreAnyGuestsCurrentlyAudible());
  if (is_currently_audible == is_currently_audible_)
    return;

  is_currently_audible_ = is_currently_audible;
  was_ever_audible_ = was_ever_audible_ || is_currently_audible_;

  SendPageMessage(
      new PageMsg_AudioStateChanged(MSG_ROUTING_NONE, is_currently_audible_));

  NotifyNavigationStateChanged(INVALIDATE_TYPE_TAB);

  if (GetOuterWebContents())
    GetOuterWebContents()->OnAudioStateChanged();

  for (auto& observer : observers_)
    observer.OnAudioStateChanged(is_currently_audible_);
}
