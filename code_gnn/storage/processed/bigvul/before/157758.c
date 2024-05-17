void WebContentsImpl::ExecuteEditCommand(
    const std::string& command,
    const base::Optional<base::string16>& value) {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->ExecuteEditCommand(command, value);
}
