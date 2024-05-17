void AudioRendererHost::DeleteEntryOnError(AudioEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  SendErrorMessage(entry->render_view_id, entry->stream_id);
  CloseAndDeleteStream(entry);
}
