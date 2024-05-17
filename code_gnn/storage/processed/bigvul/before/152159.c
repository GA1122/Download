ServiceWorkerContextCore::ProviderHostIterator::ProviderHostIterator(
    ProviderByIdMap* map,
    ProviderHostPredicate predicate)
    : map_(map),
      predicate_(std::move(predicate)),
      provider_host_iterator_(map_->begin()) {
  ForwardUntilMatchingProviderHost();
}
