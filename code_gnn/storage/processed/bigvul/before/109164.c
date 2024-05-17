void RenderViewImpl::showContextMenu(
    WebFrame* frame, const WebContextMenuData& data) {
  if (GetGuestToEmbedderChannel())
    return;

  content::ContextMenuParams params(data);

  if (params.selection_text != selection_text_) {
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

  if (params.src_url.spec().size() > content::kMaxURLChars)
    params.src_url = GURL();
  context_menu_node_ = data.node;

#if defined(OS_ANDROID)
  gfx::Rect start_rect;
  gfx::Rect end_rect;
  GetSelectionBounds(&start_rect, &end_rect);
  gfx::Point start_point(start_rect.x(),
                         start_rect.bottom());
  gfx::Point end_point(end_rect.right(),
                       end_rect.bottom());
  params.selection_start = GetScrollOffset().Add(start_point);
  params.selection_end = GetScrollOffset().Add(end_point);
#endif

  Send(new ViewHostMsg_ContextMenu(routing_id_, params));
}
