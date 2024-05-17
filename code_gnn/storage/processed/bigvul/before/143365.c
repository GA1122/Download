void ProxyResolverV8TracingFactoryImpl::CreateProxyResolverV8Tracing(
    const scoped_refptr<ProxyResolverScriptData>& pac_script,
    scoped_ptr<ProxyResolverV8Tracing::Bindings> bindings,
    scoped_ptr<ProxyResolverV8Tracing>* resolver,
    const CompletionCallback& callback,
    scoped_ptr<ProxyResolverFactory::Request>* request) {
  scoped_ptr<CreateJob> job(
      new CreateJob(this, std::move(bindings), pac_script, resolver, callback));
  jobs_.insert(job.get());
  *request = std::move(job);
}
