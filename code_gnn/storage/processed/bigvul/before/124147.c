int RenderViewHostManager::CreateRenderView(
    SiteInstance* instance,
    int opener_route_id,
    bool swapped_out) {
  CHECK(instance);

  RenderViewHostImpl* new_render_view_host = static_cast<RenderViewHostImpl*>(
      GetSwappedOutRenderViewHost(instance));
  if (new_render_view_host) {
    if (!swapped_out)
      new_render_view_host->GetProcess()->AddPendingView();
  } else {
    new_render_view_host = static_cast<RenderViewHostImpl*>(
        RenderViewHostFactory::Create(instance,
            render_view_delegate_, render_widget_delegate_, MSG_ROUTING_NONE,
            swapped_out, delegate_->
            GetControllerForRenderManager().GetSessionStorageNamespace(
                instance)));

    if (swapped_out) {
      swapped_out_hosts_[instance->GetId()] = new_render_view_host;
    } else {
      new_render_view_host->GetProcess()->AddPendingView();
    }

    bool success = InitRenderView(new_render_view_host, opener_route_id);
    if (success) {
      new_render_view_host->GetView()->Hide();

    } else if (!swapped_out) {
      CancelPending();
    }
  }

  if (!swapped_out)
    pending_render_view_host_ = new_render_view_host;

  return new_render_view_host->GetRoutingID();
}
