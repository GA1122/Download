void AudioInputRendererHost::OnStartDevice(int stream_id, int session_id) {
  VLOG(1) << "AudioInputRendererHost::OnStartDevice(stream_id="
          << stream_id << ", session_id = " << session_id << ")";

  session_entries_[session_id] = stream_id;

  media_stream_manager_->audio_input_device_manager()->Start(session_id, this);
}
