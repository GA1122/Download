void PreconnectManager::OnProxyLookupFinished(PreresolveJobId job_id,
                                              bool success) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  PreresolveJob* job = preresolve_jobs_.Lookup(job_id);
  DCHECK(job);

  if (observer_)
    observer_->OnProxyLookupFinished(job->url, success);

  job->proxy_lookup_client = nullptr;
  if (success) {
    FinishPreresolveJob(job_id, success);
  } else {
    job->resolve_host_client = PreresolveUrl(
        job->url, base::BindOnce(&PreconnectManager::OnPreresolveFinished,
                                 weak_factory_.GetWeakPtr(), job_id));
  }
}
