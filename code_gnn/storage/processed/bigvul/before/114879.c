void TestingAutomationProvider::ExecuteJavascript(
    int handle,
    const std::wstring& frame_xpath,
    const std::wstring& script,
    IPC::Message* reply_message) {
  WebContents* web_contents = GetWebContentsForHandle(handle, NULL);
  if (!web_contents) {
    AutomationMsg_DomOperation::WriteReplyParams(reply_message, std::string());
    Send(reply_message);
    return;
  }

  new DomOperationMessageSender(this, reply_message, false);
  ExecuteJavascriptInRenderViewFrame(WideToUTF16Hack(frame_xpath),
                                     WideToUTF16Hack(script), reply_message,
                                     web_contents->GetRenderViewHost());
}
