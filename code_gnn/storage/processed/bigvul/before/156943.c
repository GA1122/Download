void WebMediaPlayerMS::ActiveStateChanged(bool is_active) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (is_active)
    return;

  SetNetworkState(kNetworkStateIdle);

  if (audio_renderer_)
    audio_renderer_->Stop();
}
