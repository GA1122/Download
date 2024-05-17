void ResourceMessageFilter::OnOpenChannelToTab(
    int routing_id, int tab_id, const std::string& extension_id,
    const std::string& channel_name, int* port_id) {
  if (extensions_message_service_.get()) {
    *port_id = extensions_message_service_->
        OpenChannelToTab(routing_id, tab_id, extension_id, channel_name, this);
  } else {
    *port_id = -1;
  }
}
