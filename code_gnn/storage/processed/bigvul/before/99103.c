void ResourceDispatcherHost::OnRequestResource(
    const IPC::Message& message,
    int request_id,
    const ViewHostMsg_Resource_Request& request_data) {
  BeginRequest(request_id, request_data, NULL, message.routing_id());
}
