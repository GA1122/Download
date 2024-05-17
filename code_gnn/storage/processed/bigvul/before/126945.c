void AudioInputRendererHost::DeleteEntry(AudioEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  scoped_ptr<AudioEntry> entry_deleter(entry);

  audio_entries_.erase(entry->stream_id);
}
