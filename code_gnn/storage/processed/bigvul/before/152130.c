    ForwardUntilMatchingProviderHost() {
  while (!IsAtEnd()) {
    if (predicate_.is_null() || predicate_.Run(GetProviderHost()))
      return;
    provider_host_iterator_++;
  }
  return;
}
