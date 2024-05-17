void RenderMessageFilter::OnOpenChannelToPpapiBroker(int routing_id,
                                                     int request_id,
                                                     const FilePath& path) {
  plugin_service_->OpenChannelToPpapiBroker(
      path, new OpenChannelToPpapiBrokerCallback(this, routing_id, request_id));
}
