 bool ChromeContentClient::CanHandleWhileSwappedOut(
     const IPC::Message& msg) {
  switch (msg.type()) {
    case ChromeViewHostMsg_Snapshot::ID:
      return true;
    default:
      break;
  }
  return false;
}
