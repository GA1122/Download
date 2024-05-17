void AudioRendererHost::OnGetVolume(const IPC::Message& msg, int stream_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  NOTREACHED() << "This message shouldn't be received";
}
