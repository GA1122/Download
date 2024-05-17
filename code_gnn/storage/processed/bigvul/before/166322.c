SpeechRecognitionManagerImpl::FrameDeletionObserver::FrameDeletionObserver(
    FrameDeletedCallback frame_deleted_callback)
    : frame_deleted_callback_(std::move(frame_deleted_callback)) {}
