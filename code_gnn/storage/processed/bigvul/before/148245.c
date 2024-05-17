void BrowserPpapiHostImpl::HostMessageFilter::OnHostMsgLogInterfaceUsage(
    int hash) const {
  UMA_HISTOGRAM_SPARSE_SLOWLY("Pepper.InterfaceUsed", hash);
}
