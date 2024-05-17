void AudioRendererHost::OnPlaying(media::AudioOutputController* controller) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      base::Bind(
          &AudioRendererHost::DoSendPlayingMessage,
          this,
          make_scoped_refptr(controller)));
}
