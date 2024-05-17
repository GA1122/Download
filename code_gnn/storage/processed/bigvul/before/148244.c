void BrowserPpapiHostImpl::HostMessageFilter::OnHostDestroyed() {
  DCHECK(ppapi_host_);
  ppapi_host_ = NULL;
  browser_ppapi_host_impl_ = NULL;
}
