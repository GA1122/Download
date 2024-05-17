void AudioInputRendererHost::OnCloseStream(int stream_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupById(stream_id);

  if (entry)
    CloseAndDeleteStream(entry);

  int session_id = LookupSessionById(stream_id);

  if (session_id)
    StopAndDeleteDevice(session_id);
}
