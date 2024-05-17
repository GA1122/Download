bool WebContentsImpl::CreateRenderViewForRenderManager(
    RenderViewHost* render_view_host,
    int opener_frame_routing_id,
    int proxy_routing_id,
    const base::UnguessableToken& devtools_frame_token,
    const FrameReplicationState& replicated_frame_state) {
  TRACE_EVENT0("browser,navigation",
               "WebContentsImpl::CreateRenderViewForRenderManager");

  if (proxy_routing_id == MSG_ROUTING_NONE)
    CreateRenderWidgetHostViewForRenderManager(render_view_host);

  if (!static_cast<RenderViewHostImpl*>(render_view_host)
           ->CreateRenderView(opener_frame_routing_id, proxy_routing_id,
                              devtools_frame_token, replicated_frame_state,
                              created_with_opener_)) {
    return false;
  }

  if (proxy_routing_id == MSG_ROUTING_NONE && node_.outer_web_contents())
    ReattachToOuterWebContentsFrame();

  SetHistoryOffsetAndLengthForView(render_view_host,
                                   controller_.GetLastCommittedEntryIndex(),
                                   controller_.GetEntryCount());

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
  RenderWidgetHostView* rwh_view = render_view_host->GetWidget()->GetView();
  if (rwh_view) {
    if (RenderWidgetHost* render_widget_host = rwh_view->GetRenderWidgetHost())
      render_widget_host->WasResized();
  }
#endif

  return true;
}
