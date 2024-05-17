bool GLManager::IsFenceSyncReleased(uint64_t release) {
  return release <= command_buffer_->GetLastState().release_count;
}
