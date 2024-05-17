bool ResourceDispatcherHostImpl::OnMessageReceived(
    const IPC::Message& message,
    ResourceMessageFilter* filter) {
  filter_ = filter;
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(ResourceDispatcherHostImpl, message)
    IPC_MESSAGE_HANDLER(ResourceHostMsg_RequestResource, OnRequestResource)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(ResourceHostMsg_SyncLoad, OnSyncLoad)
    IPC_MESSAGE_HANDLER(ResourceHostMsg_ReleaseDownloadedFile,
                        OnReleaseDownloadedFile)
    IPC_MESSAGE_HANDLER(ResourceHostMsg_DataDownloaded_ACK, OnDataDownloadedACK)
    IPC_MESSAGE_HANDLER(ResourceHostMsg_CancelRequest, OnCancelRequest)
    IPC_MESSAGE_HANDLER(ResourceHostMsg_DidChangePriority, OnDidChangePriority)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  if (!handled && IPC_MESSAGE_ID_CLASS(message.type()) == ResourceMsgStart) {
    base::PickleIterator iter(message);
    int request_id = -1;
    bool ok = iter.ReadInt(&request_id);
    DCHECK(ok);
    GlobalRequestID id(filter_->child_id(), request_id);
    DelegateMap::iterator it = delegate_map_.find(id);
    if (it != delegate_map_.end()) {
      base::ObserverList<ResourceMessageDelegate>::Iterator del_it(it->second);
      ResourceMessageDelegate* delegate;
      while (!handled && (delegate = del_it.GetNext()) != NULL) {
        handled = delegate->OnMessageReceived(message);
      }
    }

    handled = true;
  }

  filter_ = NULL;
  return handled;
}
