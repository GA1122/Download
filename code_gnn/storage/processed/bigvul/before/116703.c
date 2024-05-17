void RenderViewImpl::showContextMenu(
    WebFrame* frame, const WebContextMenuData& data) {
  content::ContextMenuParams params(data);

  if (frame)
    params.frame_id = frame->identifier();

  if (params.src_url.spec().size() > content::kMaxURLChars)
    params.src_url = GURL();
  context_menu_node_ = data.node;
  Send(new ViewHostMsg_ContextMenu(routing_id_, params));
}
