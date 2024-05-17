bool BrowserPpapiHostImpl::HostMessageFilter::OnMessageReceived(
    const IPC::Message& msg) {
  if (!ppapi_host_)
    return false;

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(BrowserPpapiHostImpl::HostMessageFilter, msg)
  IPC_MESSAGE_HANDLER(PpapiHostMsg_LogInterfaceUsage,
                      OnHostMsgLogInterfaceUsage)
  IPC_MESSAGE_UNHANDLED(handled = ppapi_host_->OnMessageReceived(msg))
  IPC_END_MESSAGE_MAP()
  return handled;
}
