SpeechRecognitionManagerImpl::~SpeechRecognitionManagerImpl() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(g_speech_recognition_manager_impl);

  g_speech_recognition_manager_impl = nullptr;
}
