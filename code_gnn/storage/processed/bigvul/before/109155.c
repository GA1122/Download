void RenderViewImpl::runModalAlertDialog(WebFrame* frame,
                                         const WebString& message) {
  RunJavaScriptMessage(content::JAVASCRIPT_MESSAGE_TYPE_ALERT,
                       message,
                       string16(),
                       frame->document().url(),
                       NULL);
}
