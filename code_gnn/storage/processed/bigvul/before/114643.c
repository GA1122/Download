void WebPluginDelegateProxy::OnResolveProxy(const GURL& url,
                                            bool* result,
                                            std::string* proxy_list) {
  *result = false;
  RenderThreadImpl::current()->Send(
      new ViewHostMsg_ResolveProxy(url, result, proxy_list));
}
