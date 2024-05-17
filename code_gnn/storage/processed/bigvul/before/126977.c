void AudioRendererHost::DoSendPausedMessage(
    media::AudioOutputController* controller) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  Send(new AudioMsg_NotifyStreamStateChanged(
      entry->stream_id, media::AudioOutputIPCDelegate::kPaused));
}
