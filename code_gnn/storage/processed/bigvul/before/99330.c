void ResourceMessageFilter::OnResolveProxy(const GURL& url,
                                           IPC::Message* reply_msg) {
  resolve_proxy_msg_helper_.Start(url, reply_msg);
}
