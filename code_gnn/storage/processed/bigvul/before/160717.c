bool RenderFrameImpl::RunModalPromptDialog(
    const blink::WebString& message,
    const blink::WebString& default_value,
    blink::WebString* actual_value) {
  base::string16 result;
  bool ok = RunJavaScriptDialog(JAVASCRIPT_DIALOG_TYPE_PROMPT, message.Utf16(),
                                default_value.Utf16(),
                                frame_->GetDocument().Url(), &result);
  if (ok)
    *actual_value = WebString::FromUTF16(result);
  return ok;
}
