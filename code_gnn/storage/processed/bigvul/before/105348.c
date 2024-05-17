bool AutofillManager::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(AutofillManager, message)
    IPC_MESSAGE_HANDLER(AutofillHostMsg_FormsSeen, OnFormsSeen)
    IPC_MESSAGE_HANDLER(AutofillHostMsg_FormSubmitted, OnFormSubmitted)
    IPC_MESSAGE_HANDLER(AutofillHostMsg_QueryFormFieldAutofill,
                        OnQueryFormFieldAutofill)
    IPC_MESSAGE_HANDLER(AutofillHostMsg_ShowAutofillDialog,
                        OnShowAutofillDialog)
    IPC_MESSAGE_HANDLER(AutofillHostMsg_FillAutofillFormData,
                        OnFillAutofillFormData)
    IPC_MESSAGE_HANDLER(AutofillHostMsg_DidFillAutofillFormData,
                        OnDidFillAutofillFormData)
    IPC_MESSAGE_HANDLER(AutofillHostMsg_DidShowAutofillSuggestions,
                        OnDidShowAutofillSuggestions)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
