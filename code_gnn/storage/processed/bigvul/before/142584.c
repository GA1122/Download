WebstoreAPI::ObservedInstallInfo::ObservedInstallInfo(
    int routing_id,
    const std::string& extension_id,
    IPC::Sender* ipc_sender)
    : routing_id(routing_id),
      extension_id(extension_id),
      ipc_sender(ipc_sender) {}
