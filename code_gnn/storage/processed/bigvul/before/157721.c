void WebContentsImpl::AudioContextPlaybackStopped(RenderFrameHost* host,
                                                  int context_id) {
  WebContentsObserver::AudioContextId audio_context_id(host, context_id);
  for (auto& observer : observers_)
    observer.AudioContextPlaybackStopped(audio_context_id);
}
