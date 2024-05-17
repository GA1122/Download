bool AutoFillManager::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(AutoFillManager, message)
    IPC_MESSAGE_HANDLER(AutoFillHostMsg_FormsSeen, OnFormsSeen)
    IPC_MESSAGE_HANDLER(AutoFillHostMsg_FormSubmitted, OnFormSubmitted)
    IPC_MESSAGE_HANDLER(AutoFillHostMsg_QueryFormFieldAutoFill,
                        OnQueryFormFieldAutoFill)
    IPC_MESSAGE_HANDLER(AutoFillHostMsg_ShowAutoFillDialog,
                        OnShowAutoFillDialog)
    IPC_MESSAGE_HANDLER(AutoFillHostMsg_FillAutoFillFormData,
                        OnFillAutoFillFormData)
    IPC_MESSAGE_HANDLER(AutoFillHostMsg_DidFillAutoFillFormData,
                        OnDidFillAutoFillFormData)
    IPC_MESSAGE_HANDLER(AutoFillHostMsg_DidShowAutoFillSuggestions,
                        OnDidShowAutoFillSuggestions)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
