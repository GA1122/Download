bool RenderFrameImpl::RunModalConfirmDialog(const blink::WebString& message) {
  return RunJavaScriptDialog(JAVASCRIPT_DIALOG_TYPE_CONFIRM, message.Utf16(),
                             base::string16(), nullptr);
}
