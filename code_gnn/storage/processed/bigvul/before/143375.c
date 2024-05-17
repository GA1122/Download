void ProxyResolverV8TracingImpl::GetProxyForURL(
    const GURL& url,
    ProxyInfo* results,
    const CompletionCallback& callback,
    ProxyResolver::RequestHandle* request,
    scoped_ptr<Bindings> bindings) {
  DCHECK(CalledOnValidThread());
  DCHECK(!callback.is_null());

  scoped_refptr<Job> job = new Job(job_params_.get(), std::move(bindings));

  if (request)
    *request = job.get();

  job->StartGetProxyForURL(url, results, callback);
}
