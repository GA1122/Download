void AudioRendererHost::OnCloseStream(const IPC::Message& msg, int stream_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);

  if (entry)
    CloseAndDeleteStream(entry);
}
