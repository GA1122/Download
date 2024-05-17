IndexedDBDatabase::~IndexedDBDatabase() {
  DCHECK(!active_request_);
  DCHECK(pending_requests_.empty());
}
