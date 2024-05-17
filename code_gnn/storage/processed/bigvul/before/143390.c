void ProxyResolverV8TracingFactoryImpl::RemoveJob(
    ProxyResolverV8TracingFactoryImpl::CreateJob* job) {
  size_t erased = jobs_.erase(job);
  DCHECK_EQ(1u, erased);
}
