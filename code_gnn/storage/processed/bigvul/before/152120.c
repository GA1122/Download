void ServiceWorkerContextCore::ProviderHostIterator::Advance() {
  DCHECK(!IsAtEnd());
  provider_host_iterator_++;
  ForwardUntilMatchingProviderHost();
}
