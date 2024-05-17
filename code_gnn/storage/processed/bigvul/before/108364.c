void AudioRendererHost::DoSendPlayingMessage(
    media::AudioOutputController* controller) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  ViewMsg_AudioStreamState_Params params;
  params.state = ViewMsg_AudioStreamState_Params::kPlaying;
  Send(new ViewMsg_NotifyAudioStreamStateChanged(
      entry->render_view_id, entry->stream_id, params));
}
