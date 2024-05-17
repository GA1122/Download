void InFlightBackendIO::DoomEntryImpl(EntryImpl* entry) {
  scoped_refptr<BackendIO> operation(
      new BackendIO(this, backend_, net::CompletionCallback()));
  operation->DoomEntryImpl(entry);
  PostOperation(FROM_HERE, operation.get());
}
