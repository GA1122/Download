std::unique_ptr<SessionStore::WriteBatch> SessionStore::CreateWriteBatch(
    syncer::OnceModelErrorHandler error_handler) {
  return std::make_unique<WriteBatch>(
      store_->CreateWriteBatch(),
      base::BindOnce(&ModelTypeStore::CommitWriteBatch,
                     base::Unretained(store_.get())),
      std::move(error_handler), &session_tracker_);
}
