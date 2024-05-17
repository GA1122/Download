void InFlightBackendIO::Init(const net::CompletionCallback& callback) {
  scoped_refptr<BackendIO> operation(new BackendIO(this, backend_, callback));
  operation->Init();
  PostOperation(FROM_HERE, operation.get());
}
