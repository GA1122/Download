void WebContentsImpl::ReplaceMisspelling(const base::string16& word) {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_ReplaceMisspelling(
      focused_frame->GetRoutingID(), word));
}
