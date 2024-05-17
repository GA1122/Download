void AudioRendererHost::DeleteEntry(AudioEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  scoped_ptr<AudioEntry> entry_deleter(entry);

  audio_entries_.erase(entry->stream_id);

  if (media_observer_)
    media_observer_->OnDeleteAudioStream(this, entry->stream_id);
}
