bool RenderViewImpl::runModalConfirmDialog(WebFrame* frame,
                                           const WebString& message) {
  return RunJavaScriptMessage(content::JAVASCRIPT_MESSAGE_TYPE_CONFIRM,
                              message,
                              string16(),
                              frame->document().url(),
                              NULL);
}
