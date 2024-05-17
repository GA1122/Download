bool ResourceDispatcherHost::OnMessageReceived(const IPC::Message& message,
                                               Receiver* receiver,
                                               bool* message_was_ok) {
  if (!IsResourceDispatcherHostMessage(message)) {
    return socket_stream_dispatcher_host_->OnMessageReceived(
        message, receiver, message_was_ok);
  }

  *message_was_ok = true;
  receiver_ = receiver;

  IPC_BEGIN_MESSAGE_MAP_EX(ResourceDispatcherHost, message, *message_was_ok)
    IPC_MESSAGE_HANDLER(ViewHostMsg_RequestResource, OnRequestResource)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(ViewHostMsg_SyncLoad, OnSyncLoad)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DataReceived_ACK, OnDataReceivedACK)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UploadProgress_ACK, OnUploadProgressACK)
    IPC_MESSAGE_HANDLER(ViewHostMsg_CancelRequest, OnCancelRequest)
    IPC_MESSAGE_HANDLER(ViewHostMsg_FollowRedirect, OnFollowRedirect)
    IPC_MESSAGE_HANDLER(ViewHostMsg_ClosePage_ACK, OnClosePageACK)
  IPC_END_MESSAGE_MAP_EX()

  receiver_ = NULL;

  return true;
}
