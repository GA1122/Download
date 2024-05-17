void ResourceMessageFilter::OnOpenChannelToExtension(
    int routing_id, const std::string& source_extension_id,
    const std::string& target_extension_id,
    const std::string& channel_name, int* port_id) {
  if (extensions_message_service_.get()) {
    *port_id = extensions_message_service_->
        OpenChannelToExtension(routing_id, source_extension_id,
                               target_extension_id, channel_name, this);
  } else {
    *port_id = -1;
  }
}
