bool PasswordAutofillAgent::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(PasswordAutofillAgent, message)
    IPC_MESSAGE_HANDLER(AutofillMsg_FillPasswordForm, OnFillPasswordForm)
    IPC_MESSAGE_HANDLER(AutofillMsg_SetLoggingState, OnSetLoggingState)
    IPC_MESSAGE_HANDLER(AutofillMsg_AutofillUsernameAndPasswordDataReceived,
                        OnAutofillUsernameAndPasswordDataReceived)
    IPC_MESSAGE_HANDLER(AutofillMsg_FindFocusedPasswordForm,
                        OnFindFocusedPasswordForm)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}
