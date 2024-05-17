void CoordinatorImpl::RemovePendingResponse(
    mojom::ClientProcess* client,
    QueuedRequest::PendingResponse::Type type) {
  QueuedRequest* request = GetCurrentRequest();
  if (request == nullptr) {
    NOTREACHED() << "No current dump request.";
    return;
  }
  auto it = request->pending_responses.find({client, type});
  if (it == request->pending_responses.end()) {
    VLOG(1) << "Unexpected memory dump response";
    return;
  }
  request->pending_responses.erase(it);
}
