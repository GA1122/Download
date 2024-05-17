bool ResourceDispatcherHost::RenderViewForRequest(const URLRequest* request,
                                                  int* render_process_host_id,
                                                  int* render_view_host_id) {
  const ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);
  if (!info) {
    *render_process_host_id = -1;
    *render_view_host_id = -1;
    return false;
  }

  if (info->process_type() == ChildProcessInfo::WORKER_PROCESS) {
    const WorkerProcessHost::WorkerInstance* worker_instance =
        WorkerService::GetInstance()->FindWorkerInstance(info->child_id());
    if (!worker_instance) {
      *render_process_host_id = -1;
      *render_view_host_id = -1;
      return false;
    }
    DCHECK(!worker_instance->worker_document_set()->IsEmpty());
    const WorkerDocumentSet::DocumentInfoSet& parents =
        worker_instance->worker_document_set()->documents();
    *render_process_host_id = parents.begin()->renderer_id();
    *render_view_host_id = parents.begin()->render_view_route_id();
  } else {
    *render_process_host_id = info->child_id();
    *render_view_host_id = info->route_id();
  }
  return true;
}
