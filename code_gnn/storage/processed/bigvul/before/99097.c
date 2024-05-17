void ResourceDispatcherHost::OnClosePageACK(
    const ViewMsg_ClosePage_Params& params) {
  if (params.for_cross_site_transition) {
    GlobalRequestID global_id(params.new_render_process_host_id,
                              params.new_request_id);
    PendingRequestList::iterator i = pending_requests_.find(global_id);
    if (i != pending_requests_.end()) {
      ResourceDispatcherHostRequestInfo* info = InfoForRequest(i->second);
      if (info->cross_site_handler())
        info->cross_site_handler()->ResumeResponse();
    }
  } else {
    DCHECK(params.new_render_process_host_id == -1);
    DCHECK(params.new_request_id == -1);
    CallRenderViewHost(params.closing_process_id,
                       params.closing_route_id,
                       &RenderViewHost::ClosePageIgnoringUnloadEvents);
  }
}
