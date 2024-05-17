void AudioRendererHost::OnPauseStream(const IPC::Message& msg, int stream_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);
  if (!entry) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  entry->controller->Pause();
}
