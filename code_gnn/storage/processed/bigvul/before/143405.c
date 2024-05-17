ProxyResolverV8TracingImpl::~ProxyResolverV8TracingImpl() {
  CHECK_EQ(0, num_outstanding_callbacks_);

  base::ThreadRestrictions::ScopedAllowIO allow_io;
  thread_.reset();
}
