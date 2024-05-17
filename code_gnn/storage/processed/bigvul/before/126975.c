void AudioRendererHost::DeleteEntryOnError(AudioEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  SendErrorMessage(entry->stream_id);

  if (media_observer_)
    media_observer_->OnSetAudioStreamStatus(this, entry->stream_id, "error");
  CloseAndDeleteStream(entry);
}
