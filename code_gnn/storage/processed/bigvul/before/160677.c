void RenderFrameImpl::OnReplaceMisspelling(const base::string16& text) {
  if (!frame_->HasSelection())
    return;

  frame_->ReplaceMisspelledRange(WebString::FromUTF16(text));
}
