  void Initialize(unsigned long resource_id, const GURL& url, int notify_id) {
    resource_id_ = resource_id;
    channel_->Send(new PluginMsg_HandleURLRequestReply(
        instance_id_, resource_id, url, notify_id));
  }
