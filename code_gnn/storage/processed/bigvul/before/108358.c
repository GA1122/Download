void AudioRendererHost::DeleteEntry(AudioEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  scoped_ptr<AudioEntry> entry_deleter(entry);

  audio_entries_.erase(
      AudioEntryId(entry->render_view_id, entry->stream_id));
}
