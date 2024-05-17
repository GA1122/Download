  void InitializeForSeekableStream(unsigned long resource_id,
                                   int range_request_id) {
    resource_id_ = resource_id;
    multibyte_response_expected_ = true;
    channel_->Send(new PluginMsg_HTTPRangeRequestReply(
        instance_id_, resource_id, range_request_id));
  }
