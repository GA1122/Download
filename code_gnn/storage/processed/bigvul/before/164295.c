 bool PageCaptureSaveAsMHTMLFunction::OnMessageReceived(
     const IPC::Message& message) {
   if (message.type() != ExtensionHostMsg_ResponseAck::ID)
    return false;

  int message_request_id;
  base::PickleIterator iter(message);
  if (!iter.ReadInt(&message_request_id)) {
    NOTREACHED() << "malformed extension message";
    return true;
  }

  if (message_request_id != request_id())
    return false;

  Release();   

  return true;
}
