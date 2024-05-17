WebPluginDelegateProxy::CreateResourceClient(
    unsigned long resource_id, const GURL& url, int notify_id) {
  if (!channel_host_)
    return NULL;

  ResourceClientProxy* proxy = new ResourceClientProxy(channel_host_,
                                                       instance_id_);
  proxy->Initialize(resource_id, url, notify_id);
  return proxy;
}
