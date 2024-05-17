void InFlightBackendIO::WriteSparseData(
    EntryImpl* entry,
    int64_t offset,
    net::IOBuffer* buf,
    int buf_len,
    const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->WriteSparseData(entry, offset, buf, buf_len);
  PostOperation(FROM_HERE, operation.get());
}
