void IndexedDBConnection::Close() {
  if (!callbacks_.get())
    return;
  base::WeakPtr<IndexedDBConnection> this_obj = weak_factory_.GetWeakPtr();
  database_->Close(this, false  );
  if (this_obj) {
    database_ = nullptr;
    callbacks_ = nullptr;
    active_observers_.clear();
  }
}
