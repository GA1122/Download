void ExtensionFunctionDispatcher::SendAccessDenied(
    IPC::Message::Sender* ipc_sender, int routing_id, int request_id) {
  ipc_sender->Send(new ExtensionMsg_Response(
      routing_id, request_id, false, std::string(),
      "Access to extension API denied."));
}
