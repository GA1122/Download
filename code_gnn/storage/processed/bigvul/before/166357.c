SpeechRecognitionManagerImpl::FrameDeletionObserver::~FrameDeletionObserver() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  DCHECK_EQ(0u, contents_observers_.size());
}
