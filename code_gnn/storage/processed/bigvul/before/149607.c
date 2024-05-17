void PreconnectManager::StartPreresolveHost(const GURL& url) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  if (!url.SchemeIsHTTPOrHTTPS())
    return;
  PreresolveJobId job_id = preresolve_jobs_.Add(std::make_unique<PreresolveJob>(
      url.GetOrigin(), 0, kAllowCredentialsOnPreconnectByDefault,
      net::NetworkIsolationKey(), nullptr));
  queued_jobs_.push_front(job_id);

  TryToLaunchPreresolveJobs();
}
