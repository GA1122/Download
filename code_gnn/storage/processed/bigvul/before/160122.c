void InFlightBackendIO::DoomAllEntries(
    const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->DoomAllEntries();
  PostOperation(FROM_HERE, operation.get());
}
