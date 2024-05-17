void AudioRendererHost::CloseAndDeleteStream(AudioEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  if (!entry->pending_close) {
    entry->controller->Close(
        base::Bind(&AudioRendererHost::DeleteEntry, this, entry));
    entry->pending_close = true;
  }
}
