void InFlightBackendIO::EndEnumeration(
    std::unique_ptr<Rankings::Iterator> iterator) {
  scoped_refptr<BackendIO> operation(
      new BackendIO(this, backend_, net::CompletionCallback()));
  operation->EndEnumeration(std::move(iterator));
  PostOperation(FROM_HERE, operation.get());
}
