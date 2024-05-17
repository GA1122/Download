void InFlightBackendIO::ReadData(EntryImpl* entry, int index, int offset,
                                 net::IOBuffer* buf, int buf_len,
                                 const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->ReadData(entry, index, offset, buf, buf_len);
  PostOperation(FROM_HERE, operation.get());
}
