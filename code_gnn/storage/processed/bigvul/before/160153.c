void InFlightBackendIO::OpenNextEntry(Rankings::Iterator* iterator,
                                      Entry** next_entry,
                                      const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->OpenNextEntry(iterator, next_entry);
  PostOperation(FROM_HERE, operation.get());
}
