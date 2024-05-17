void RenderFrameImpl::OnClearActiveFindMatch() {
  frame_->ExecuteCommand(WebString::FromUTF8("CollapseSelection"));
  frame_->ClearActiveFindMatch();
}
