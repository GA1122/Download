void InFlightBackendIO::ReadyForSparseIO(
    EntryImpl* entry, const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->ReadyForSparseIO(entry);
  PostOperation(FROM_HERE, operation.get());
}
