void WebContentsImpl::ShowContextMenu(RenderFrameHost* render_frame_host,
                                      const ContextMenuParams& params) {
  if (showing_context_menu_)
    return;

  ContextMenuParams context_menu_params(params);
  if (delegate_ && delegate_->HandleContextMenu(context_menu_params))
    return;

  render_view_host_delegate_view_->ShowContextMenu(render_frame_host,
                                                   context_menu_params);
}
