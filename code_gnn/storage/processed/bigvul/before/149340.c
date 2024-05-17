void IndexedDBConnection::VersionChangeIgnored() {
  if (!database_.get())
    return;
  database_->VersionChangeIgnored();
}
