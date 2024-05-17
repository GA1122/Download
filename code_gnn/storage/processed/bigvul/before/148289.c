bool PrintViewManager::OnMessageReceived(
    const IPC::Message& message,
    content::RenderFrameHost* render_frame_host) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_WITH_PARAM(PrintViewManager, message, render_frame_host)
    IPC_MESSAGE_HANDLER(PrintHostMsg_DidShowPrintDialog, OnDidShowPrintDialog)
    IPC_MESSAGE_HANDLER_WITH_PARAM_DELAY_REPLY(
        PrintHostMsg_SetupScriptedPrintPreview, OnSetupScriptedPrintPreview)
    IPC_MESSAGE_HANDLER(PrintHostMsg_ShowScriptedPrintPreview,
                        OnShowScriptedPrintPreview)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled ||
         PrintViewManagerBase::OnMessageReceived(message, render_frame_host);
}
