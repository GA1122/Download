  void WebPluginDelegateProxy::DidFinishLoadWithReason(
    const GURL& url, NPReason reason, intptr_t notify_data) {
//     const GURL& url, NPReason reason, int notify_id) {
    Send(new PluginMsg_DidFinishLoadWithReason(
      instance_id_, url, reason, notify_data));
//       instance_id_, url, reason, notify_id));
  }