void RenderFrameImpl::OnPaste() {
  AutoResetMember<bool> handling_select_range(
      this, &RenderFrameImpl::handling_select_range_, true);
  AutoResetMember<bool> handling_paste(this, &RenderFrameImpl::is_pasting_,
                                       true);
  frame_->ExecuteCommand(WebString::FromUTF8("Paste"));
}
