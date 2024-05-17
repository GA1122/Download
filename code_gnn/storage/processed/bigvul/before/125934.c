DomOperationMessageSender::DomOperationMessageSender(
    AutomationProvider* automation,
    IPC::Message* reply_message,
    bool use_json_interface)
    : DomOperationObserver(0),
      automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      use_json_interface_(use_json_interface) {
}
