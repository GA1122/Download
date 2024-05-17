void WebContentsImpl::SelectRange(const gfx::Point& base,
                                  const gfx::Point& extent) {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->SelectRange(base, extent);
}
