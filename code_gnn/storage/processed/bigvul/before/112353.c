void ResourceDispatcherHostImpl::OnRequestResource(
    const IPC::Message& message,
    int request_id,
    const ResourceHostMsg_Request& request_data) {
  BeginRequest(request_id, request_data, NULL, message.routing_id());
}
