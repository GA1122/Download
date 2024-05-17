void AudioRendererHost::DoRequestMoreData(
    media::AudioOutputController* controller,
    AudioBuffersState buffers_state) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry || entry->pending_buffer_request)
    return;

  DCHECK(!entry->controller->LowLatencyMode());
  entry->pending_buffer_request = true;
  Send(new ViewMsg_RequestAudioPacket(
      entry->render_view_id, entry->stream_id, buffers_state));
}
