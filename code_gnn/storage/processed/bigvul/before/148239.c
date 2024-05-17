BrowserPpapiHostImpl::HostMessageFilter::HostMessageFilter(
    ppapi::host::PpapiHost* ppapi_host,
    BrowserPpapiHostImpl* browser_ppapi_host_impl)
    : ppapi_host_(ppapi_host),
      browser_ppapi_host_impl_(browser_ppapi_host_impl) {}
