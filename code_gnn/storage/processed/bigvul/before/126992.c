void AudioRendererHost::OnSetVolume(int stream_id, double volume) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupById(stream_id);
  if (!entry) {
    SendErrorMessage(stream_id);
    return;
  }

  if (volume < 0 || volume > 1.0)
    return;
  entry->controller->SetVolume(volume);
  if (media_observer_)
    media_observer_->OnSetAudioStreamVolume(this, stream_id, volume);
}
