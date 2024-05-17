LoadState ProxyResolverV8TracingImpl::GetLoadState(
    ProxyResolver::RequestHandle request) const {
  Job* job = reinterpret_cast<Job*>(request);
  return job->GetLoadState();
}
