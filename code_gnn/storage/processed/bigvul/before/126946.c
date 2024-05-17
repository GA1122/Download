void AudioInputRendererHost::DeleteEntryOnError(AudioEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  SendErrorMessage(entry->stream_id);
  CloseAndDeleteStream(entry);
}
