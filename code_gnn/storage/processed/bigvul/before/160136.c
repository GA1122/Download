void InFlightBackendIO::FlushQueue(const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->FlushQueue();
  PostOperation(FROM_HERE, operation.get());
}
