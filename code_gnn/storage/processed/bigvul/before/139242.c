base::SingleThreadTaskRunner* AudioSystemImpl::GetTaskRunner() const {
  return audio_manager_->GetTaskRunner();
}
