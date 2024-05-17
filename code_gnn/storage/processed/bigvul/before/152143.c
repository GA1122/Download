bool ServiceWorkerContextCore::ProviderHostIterator::IsAtEnd() {
  return provider_host_iterator_ == map_->end();
}
