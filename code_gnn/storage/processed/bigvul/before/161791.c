PlatformSensorProviderBase::CloneSharedBufferHandle() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  CreateSharedBufferIfNeeded();
  return shared_buffer_handle_->Clone(
      mojo::SharedBufferHandle::AccessMode::READ_ONLY);
}
