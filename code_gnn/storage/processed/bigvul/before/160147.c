void InFlightBackendIO::OnExternalCacheHit(const std::string& key) {
  scoped_refptr<BackendIO> operation(
      new BackendIO(this, backend_, net::CompletionCallback()));
  operation->OnExternalCacheHit(key);
  PostOperation(FROM_HERE, operation.get());
}
