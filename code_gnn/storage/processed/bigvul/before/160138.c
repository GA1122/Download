void InFlightBackendIO::GetAvailableRange(
    EntryImpl* entry,
    int64_t offset,
    int len,
    int64_t* start,
    const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->GetAvailableRange(entry, offset, len, start);
  PostOperation(FROM_HERE, operation.get());
}
