void TestingAutomationProvider::AcceptOrDismissAppModalDialog(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  bool accept;
  if (!args->GetBoolean("accept", &accept)) {
    reply.SendError("Missing or invalid 'accept'");
    return;
  }

  ErrorCode code;
  JavaScriptAppModalDialog* dialog = GetActiveJavaScriptModalDialog(&code);
  if (!dialog) {
    reply.SendErrorCode(code);
    return;
  }
  if (accept) {
    std::string prompt_text;
    if (args->GetString("prompt_text", &prompt_text))
      dialog->SetOverridePromptText(UTF8ToUTF16(prompt_text));
    dialog->native_dialog()->AcceptAppModalDialog();
  } else {
    dialog->native_dialog()->CancelAppModalDialog();
  }
  reply.SendSuccess(NULL);
}
