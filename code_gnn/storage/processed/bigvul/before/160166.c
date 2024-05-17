void InFlightBackendIO::WriteData(EntryImpl* entry, int index, int offset,
                                  net::IOBuffer* buf, int buf_len,
                                  bool truncate,
                                  const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->WriteData(entry, index, offset, buf, buf_len, truncate);
  PostOperation(FROM_HERE, operation.get());
}
