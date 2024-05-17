void RenderView::showContextMenu(
    WebFrame* frame, const WebContextMenuData& data) {
  ContextMenuParams params = ContextMenuParams(data);
  if (params.src_url.spec().size() > content::kMaxURLChars)
    params.src_url = GURL();
  context_menu_node_ = data.node;
  Send(new ViewHostMsg_ContextMenu(routing_id_, params));
}
