void ResourceDispatcherHostImpl::OnSyncLoad(
    int request_id,
    const ResourceHostMsg_Request& request_data,
    IPC::Message* sync_result) {
  BeginRequest(request_id, request_data, sync_result,
                sync_result->routing_id());
 }
