ServiceWorkerDevToolsAgentHost::ServiceWorkerDevToolsAgentHost(
    int worker_process_id,
    int worker_route_id,
    const ServiceWorkerContextCore* context,
    base::WeakPtr<ServiceWorkerContextCore> context_weak,
    int64_t version_id,
    const GURL& url,
    const GURL& scope,
    bool is_installed_version,
    const base::UnguessableToken& devtools_worker_token)
    : DevToolsAgentHostImpl(devtools_worker_token.ToString()),
      state_(WORKER_NOT_READY),
      devtools_worker_token_(devtools_worker_token),
      worker_process_id_(worker_process_id),
      worker_route_id_(worker_route_id),
      context_(context),
      context_weak_(context_weak),
      version_id_(version_id),
      url_(url),
      scope_(scope),
      version_installed_time_(is_installed_version ? base::Time::Now()
                                                   : base::Time()) {
  NotifyCreated();
}
