int RenderViewImpl::ShowContextMenu(ContextMenuClient* client,
                                    const ContextMenuParams& params) {
  DCHECK(client);   
  ContextMenuParams our_params(params);
  our_params.custom_context.request_id = pending_context_menus_.Add(client);
  Send(new ViewHostMsg_ContextMenu(routing_id_, our_params));
  return our_params.custom_context.request_id;
}
