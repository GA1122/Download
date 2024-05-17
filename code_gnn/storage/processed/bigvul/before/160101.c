void BackendImpl::SyncEndEnumeration(
    std::unique_ptr<Rankings::Iterator> iterator) {
  iterator->Reset();
}
