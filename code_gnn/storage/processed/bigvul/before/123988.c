bool RenderViewImpl::runModalPromptDialog(WebFrame* frame,
                                          const WebString& message,
                                          const WebString& default_value,
                                          WebString* actual_value) {
  string16 result;
  bool ok = RunJavaScriptMessage(JAVASCRIPT_MESSAGE_TYPE_PROMPT,
                                 message,
                                 default_value,
                                 frame->document().url(),
                                 &result);
  if (ok)
    actual_value->assign(result);
  return ok;
}
