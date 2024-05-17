void BaseAudioContext::NotifySourceNodeFinishedProcessing(
    AudioHandler* handler) {
  MutexLocker lock(finished_source_handlers_mutex_);
  finished_source_handlers_.push_back(handler);
}
