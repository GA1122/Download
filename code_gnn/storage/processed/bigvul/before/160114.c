void InFlightBackendIO::CalculateSizeOfAllEntries(
    const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->CalculateSizeOfAllEntries();
  PostOperation(FROM_HERE, operation.get());
}
