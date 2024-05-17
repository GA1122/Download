void IndexedDBDatabase::VersionChangeIgnored() {
  if (active_request_)
    active_request_->OnVersionChangeIgnored();
}
