void AudioRendererHost::OnSetVolume(int stream_id, double volume) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  AudioOutputDelegate* delegate = LookupById(stream_id);
  if (!delegate) {
    SendErrorMessage(stream_id);
    return;
  }

  if (volume < 0 || volume > 1.0)
    return;
  delegate->OnSetVolume(volume);
}
