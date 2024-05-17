void GLManager::SignalSyncToken(const gpu::SyncToken& sync_token,
                                base::OnceClosure callback) {
  command_buffer_->SignalSyncToken(
      sync_token, base::AdaptCallbackForRepeating(std::move(callback)));
}
