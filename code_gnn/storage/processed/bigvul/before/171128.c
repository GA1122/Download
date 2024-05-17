CallbackThread::CallbackThread(
 const wp<MediaPlayerBase::AudioSink> &sink,
 MediaPlayerBase::AudioSink::AudioCallback cb,
 void *cookie)
 : mSink(sink),
      mCallback(cb),
      mCookie(cookie),
      mBuffer(NULL),
      mBufferSize(0) {
}
