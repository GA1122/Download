void ProxyResolverV8TracingImpl::CancelRequest(
    ProxyResolver::RequestHandle request) {
  Job* job = reinterpret_cast<Job*>(request);
  job->Cancel();
}
