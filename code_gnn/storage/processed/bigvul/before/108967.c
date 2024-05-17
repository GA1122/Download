void RenderViewImpl::OnSelectRange(const gfx::Point& start,
                                   const gfx::Point& end) {
  if (!webview())
    return;

  Send(new ViewHostMsg_SelectRange_ACK(routing_id_));

  handling_select_range_ = true;
  webview()->focusedFrame()->selectRange(start, end);
  handling_select_range_ = false;
}
