void GetDevToolsRouteInfoOnIO(
    scoped_refptr<ServiceWorkerContextWrapper> context,
    int64_t version_id,
    const base::Callback<void(int, int)>& callback) {
  if (content::ServiceWorkerVersion* version =
          context->GetLiveVersion(version_id)) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(
            callback, version->embedded_worker()->process_id(),
            version->embedded_worker()->worker_devtools_agent_route_id()));
  }
}
