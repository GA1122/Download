  WebPluginDelegateProxy::CreateResourceClient(
    unsigned long resource_id, const GURL& url, bool notify_needed,
    intptr_t notify_data, intptr_t npstream) {
//     unsigned long resource_id, const GURL& url, int notify_id) {
//   if (!channel_host_)
//     return NULL;
// 
//   ResourceClientProxy* proxy = new ResourceClientProxy(channel_host_,
//                                                        instance_id_);
//   proxy->Initialize(resource_id, url, notify_id);
//   return proxy;
// }
// 
// webkit_glue::WebPluginResourceClient*
// WebPluginDelegateProxy::CreateSeekableResourceClient(
//     unsigned long resource_id, int range_request_id) {
    if (!channel_host_)
      return NULL;
  
    ResourceClientProxy* proxy = new ResourceClientProxy(channel_host_,
                                                         instance_id_);
  proxy->Initialize(resource_id, url, notify_needed, notify_data, npstream);
//   proxy->InitializeForSeekableStream(resource_id, range_request_id);
    return proxy;
  }