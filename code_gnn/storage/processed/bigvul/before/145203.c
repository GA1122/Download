bool WebstoreBindings::OnMessageReceived(const IPC::Message& message) {
  IPC_BEGIN_MESSAGE_MAP(WebstoreBindings, message)
    IPC_MESSAGE_HANDLER(ExtensionMsg_InlineWebstoreInstallResponse,
                        OnInlineWebstoreInstallResponse)
    IPC_MESSAGE_HANDLER(ExtensionMsg_InlineInstallStageChanged,
                        OnInlineInstallStageChanged)
    IPC_MESSAGE_HANDLER(ExtensionMsg_InlineInstallDownloadProgress,
                        OnInlineInstallDownloadProgress)
    IPC_MESSAGE_UNHANDLED(CHECK(false) << "Unhandled IPC message")
  IPC_END_MESSAGE_MAP()
  return true;
}
