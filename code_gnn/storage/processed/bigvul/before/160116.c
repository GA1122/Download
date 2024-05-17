void InFlightBackendIO::CancelSparseIO(EntryImpl* entry) {
  scoped_refptr<BackendIO> operation(
      new BackendIO(this, backend_, net::CompletionCallback()));
  operation->CancelSparseIO(entry);
  PostOperation(FROM_HERE, operation.get());
}
