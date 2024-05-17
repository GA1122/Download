void AudioRendererHost::CloseAndDeleteStream(AudioEntry* entry) {
  if (!entry->pending_close) {
    entry->controller->Close(
        NewRunnableMethod(this, &AudioRendererHost::OnStreamClosed, entry));
    entry->pending_close = true;
  }
}
