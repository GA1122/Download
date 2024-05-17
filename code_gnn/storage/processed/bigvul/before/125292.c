void RenderViewImpl::showContextMenu(
    WebFrame* frame, const WebContextMenuData& data) {
  ContextMenuParams params(data);

  string16 selection_text;
  if (!selection_text_.empty() && !selection_range_.is_empty()) {
    const int start = selection_range_.GetMin() - selection_text_offset_;
    const size_t length = selection_range_.length();
    if (start >= 0 && start + length <= selection_text_.length())
      selection_text = selection_text_.substr(start, length);
  }
  if (params.selection_text != selection_text) {
    selection_text_ = params.selection_text;
    selection_text_offset_ = 0;
    selection_range_ = ui::Range(0, selection_text_.length());
    Send(new ViewHostMsg_SelectionChanged(routing_id_,
                                          selection_text_,
                                          selection_text_offset_,
                                          selection_range_));
  }

  if (frame)
    params.frame_id = frame->identifier();

  if (params.src_url.spec().size() > kMaxURLChars)
    params.src_url = GURL();
  context_menu_node_ = data.node;

#if defined(OS_ANDROID)
  gfx::Rect start_rect;
  gfx::Rect end_rect;
  GetSelectionBounds(&start_rect, &end_rect);
  params.selection_start =
      gfx::Point(start_rect.x(), start_rect.bottom()) + GetScrollOffset();
  params.selection_end =
      gfx::Point(end_rect.right(), end_rect.bottom()) + GetScrollOffset();
#endif

  Send(new ViewHostMsg_ContextMenu(routing_id_, params));

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidRequestShowContextMenu(frame, data));
}
