void ResourceDispatcherHostImpl::OnSwapOutACK(
    const ViewMsg_SwapOut_Params& params) {
  GlobalRequestID global_id(params.new_render_process_host_id,
                            params.new_request_id);
  PendingRequestList::iterator i = pending_requests_.find(global_id);
  if (i != pending_requests_.end()) {
    ResourceRequestInfoImpl* info =
        ResourceRequestInfoImpl::ForRequest(i->second);
    if (info->cross_site_handler())
      info->cross_site_handler()->ResumeResponse();
  }
  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(&OnSwapOutACKHelper,
                 params.closing_process_id,
                 params.closing_route_id));
}
