void ExecuteJavascriptInRenderViewFrame(
    const string16& frame_xpath,
    const string16& script,
    IPC::Message* reply_message,
    RenderViewHost* render_view_host) {
  std::string set_automation_id;
  base::SStringPrintf(&set_automation_id,
                      "window.domAutomationController.setAutomationId(%d);",
                      reply_message->routing_id());

  render_view_host->ExecuteJavascriptInWebFrame(
      frame_xpath, UTF8ToUTF16(set_automation_id));
  render_view_host->ExecuteJavascriptInWebFrame(
      frame_xpath, script);
}
