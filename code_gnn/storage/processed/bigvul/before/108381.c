void AudioRendererHost::OnPlaying(media::AudioOutputController* controller) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      NewRunnableMethod(
          this,
          &AudioRendererHost::DoSendPlayingMessage,
          make_scoped_refptr(controller)));
}
