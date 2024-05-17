void AudioInputRendererHost::OnError(
    media::AudioInputController* controller,
    int error_code) {
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      base::Bind(&AudioInputRendererHost::DoHandleError,
          this, make_scoped_refptr(controller), error_code));
}
