void AudioInputRendererHost::StopAndDeleteDevice(int session_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  media_stream_manager_->audio_input_device_manager()->Stop(session_id);

  session_entries_.erase(session_id);
}
