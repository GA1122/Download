PendingHostCreator::PendingHostCreator(BrowserPpapiHostImpl* host,
                                       BrowserMessageFilter* connection,
                                       int routing_id,
                                       int sequence_id,
                                       size_t nested_msgs_size)
    : host_(host),
      connection_(connection),
      routing_id_(routing_id),
      sequence_id_(sequence_id),
      pending_resource_host_ids_(nested_msgs_size, 0) {}
