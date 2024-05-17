void PreconnectManager::OnPreresolveFinished(PreresolveJobId job_id,
                                             bool success) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  PreresolveJob* job = preresolve_jobs_.Lookup(job_id);
  DCHECK(job);

  if (observer_)
    observer_->OnPreresolveFinished(job->url, success);

  job->resolve_host_client = nullptr;
  FinishPreresolveJob(job_id, success);
}
