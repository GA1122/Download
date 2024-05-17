void AudioRendererHost::OnPaused(media::AudioOutputController* controller) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      NewRunnableMethod(
          this,
          &AudioRendererHost::DoSendPausedMessage,
          make_scoped_refptr(controller)));
}
