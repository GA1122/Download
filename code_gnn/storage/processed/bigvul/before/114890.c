void TestingAutomationProvider::GetAppModalDialogMessage(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  ErrorCode code;
  JavaScriptAppModalDialog* dialog = GetActiveJavaScriptModalDialog(&code);
  if (!dialog) {
    reply.SendErrorCode(code);
    return;
  }
  DictionaryValue result_dict;
  result_dict.SetString("message", UTF16ToUTF8(dialog->message_text()));
  reply.SendSuccess(&result_dict);
}
