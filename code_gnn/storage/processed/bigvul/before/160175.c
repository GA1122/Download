std::unique_ptr<Backend::Iterator> MemBackendImpl::CreateIterator() {
  return std::unique_ptr<Backend::Iterator>(
      new MemIterator(weak_factory_.GetWeakPtr()));
}
