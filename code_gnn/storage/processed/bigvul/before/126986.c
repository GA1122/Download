void AudioRendererHost::OnFlushStream(int stream_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupById(stream_id);
  if (!entry) {
    SendErrorMessage(stream_id);
    return;
  }

  entry->controller->Flush();
  if (media_observer_)
    media_observer_->OnSetAudioStreamStatus(this, stream_id, "flushed");
}
