  void WillSendRequest(const GURL& url, int http_status_code) {
    DCHECK(channel_ != NULL);
    channel_->Send(new PluginMsg_WillSendRequest(instance_id_, resource_id_,
                                                 url, http_status_code));
  }