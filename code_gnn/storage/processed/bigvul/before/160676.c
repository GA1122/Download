void RenderFrameImpl::OnReplace(const base::string16& text) {
  if (!frame_->HasSelection())
    frame_->SelectWordAroundCaret();

  frame_->ReplaceSelection(WebString::FromUTF16(text));
  SyncSelectionIfRequired();
}
