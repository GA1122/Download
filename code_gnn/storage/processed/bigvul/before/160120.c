void InFlightBackendIO::CreateEntry(const std::string& key, Entry** entry,
                                    const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->CreateEntry(key, entry);
  PostOperation(FROM_HERE, operation.get());
}
