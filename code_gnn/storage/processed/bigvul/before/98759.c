WebPluginDelegateProxy::CreateSeekableResourceClient(
    unsigned long resource_id, int range_request_id) {
  if (!channel_host_)
    return NULL;

  ResourceClientProxy* proxy = new ResourceClientProxy(channel_host_,
                                                       instance_id_);
  proxy->InitializeForSeekableStream(resource_id, range_request_id);
  return proxy;
}
