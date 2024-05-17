void AudioInputRendererHost::OnDeviceStopped(int session_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  SessionEntryMap::iterator it = session_entries_.find(session_id);
  if (it == session_entries_.end())
    return;

  int stream_id = it->second;
  AudioEntry* entry = LookupById(stream_id);

  if (entry) {
    CloseAndDeleteStream(entry);
    Send(new AudioInputMsg_NotifyStreamStateChanged(
        stream_id, media::AudioInputIPCDelegate::kStopped));
  }

  session_entries_.erase(it);
}
