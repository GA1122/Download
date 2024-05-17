BrowserPpapiHostImpl* PepperRendererConnection::GetHostForChildProcess(
    int child_process_id) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  BrowserPpapiHostImpl* host = static_cast<BrowserPpapiHostImpl*>(
      GetContentClient()->browser()->GetExternalBrowserPpapiHost(
          child_process_id));

  if (!host) {
    for (PpapiPluginProcessHostIterator iter; !iter.Done(); ++iter) {
      if (iter->process() &&
          iter->process()->GetData().id == child_process_id) {
        host = iter->host_impl();
        break;
      }
    }
  }

  if (!host && child_process_id == 0) {
    host = in_process_host_.get();
  }

  return host;
}
