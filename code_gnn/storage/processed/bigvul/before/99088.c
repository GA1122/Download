bool ResourceDispatcherHost::IsResourceDispatcherHostMessage(
    const IPC::Message& message) {
  switch (message.type()) {
    case ViewHostMsg_RequestResource::ID:
    case ViewHostMsg_CancelRequest::ID:
    case ViewHostMsg_FollowRedirect::ID:
    case ViewHostMsg_ClosePage_ACK::ID:
    case ViewHostMsg_DataReceived_ACK::ID:
    case ViewHostMsg_DownloadProgress_ACK::ID:
    case ViewHostMsg_UploadProgress_ACK::ID:
    case ViewHostMsg_SyncLoad::ID:
      return true;

    default:
      break;
  }

  return false;
}
