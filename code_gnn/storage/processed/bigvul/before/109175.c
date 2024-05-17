void RenderViewImpl::willOpenSocketStream(
    WebSocketStreamHandle* handle) {
  SocketStreamHandleData::AddToHandle(handle, routing_id_);
}
