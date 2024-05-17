void RenderFrameHostImpl::CreateNewWindow(
    mojom::CreateNewWindowParamsPtr params,
    const CreateNewWindowCallback& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  bool no_javascript_access = false;

  bool can_create_window =
      frame_tree_node_->current_frame_host() == this && render_frame_created_ &&
      GetContentClient()->browser()->CanCreateWindow(
          this, params->opener_url, params->opener_top_level_frame_url,
          params->opener_security_origin, params->window_container_type,
          params->target_url, params->referrer, params->frame_name,
          params->disposition, *params->features, params->user_gesture,
          params->opener_suppressed, &no_javascript_access);

  mojom::CreateNewWindowReplyPtr reply = mojom::CreateNewWindowReply::New();
  if (!can_create_window) {
    RunCreateWindowCompleteCallback(callback, std::move(reply),
                                    MSG_ROUTING_NONE, MSG_ROUTING_NONE,
                                    MSG_ROUTING_NONE, 0);
    return;
  }


  StoragePartition* storage_partition = BrowserContext::GetStoragePartition(
      GetSiteInstance()->GetBrowserContext(), GetSiteInstance());
  DOMStorageContextWrapper* dom_storage_context =
      static_cast<DOMStorageContextWrapper*>(
          storage_partition->GetDOMStorageContext());
  auto cloned_namespace = base::MakeShared<SessionStorageNamespaceImpl>(
      dom_storage_context, params->session_storage_namespace_id);
  reply->cloned_session_storage_namespace_id = cloned_namespace->id();

  int render_view_route_id = MSG_ROUTING_NONE;
  int main_frame_route_id = MSG_ROUTING_NONE;
  int main_frame_widget_route_id = MSG_ROUTING_NONE;
  int render_process_id = GetProcess()->GetID();
  if (!params->opener_suppressed && !no_javascript_access) {
    render_view_route_id = GetProcess()->GetNextRoutingID();
    main_frame_route_id = GetProcess()->GetNextRoutingID();
    main_frame_widget_route_id = render_view_route_id;
    auto block_requests_for_route = base::Bind(
        [](const GlobalFrameRoutingId& id) {
          auto* rdh = ResourceDispatcherHostImpl::Get();
          if (rdh)
            rdh->BlockRequestsForRoute(id);
        },
        GlobalFrameRoutingId(render_process_id, main_frame_route_id));
    BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                            block_requests_for_route);
  }

  DCHECK(IsRenderFrameLive());

  mojom::CreateNewWindowParamsPtr validated_params(params.Clone());
  GetProcess()->FilterURL(false, &validated_params->target_url);

  GetProcess()->FilterURL(false, &validated_params->opener_url);
  GetProcess()->FilterURL(true, &validated_params->opener_security_origin);

  delegate_->CreateNewWindow(this, render_view_route_id, main_frame_route_id,
                             main_frame_widget_route_id, *validated_params,
                             cloned_namespace.get());

  if (main_frame_route_id != MSG_ROUTING_NONE) {
    bool succeeded =
        RenderWidgetHost::FromID(render_process_id,
                                 main_frame_widget_route_id) != nullptr;
    if (!succeeded) {
      DCHECK(!RenderFrameHost::FromID(render_process_id, main_frame_route_id));
      DCHECK(!RenderViewHost::FromID(render_process_id, render_view_route_id));
      RunCreateWindowCompleteCallback(callback, std::move(reply),
                                      MSG_ROUTING_NONE, MSG_ROUTING_NONE,
                                      MSG_ROUTING_NONE, 0);
      return;
    }
    DCHECK(RenderFrameHost::FromID(render_process_id, main_frame_route_id));
    DCHECK(RenderViewHost::FromID(render_process_id, render_view_route_id));
  }

  RunCreateWindowCompleteCallback(
      callback, std::move(reply), render_view_route_id, main_frame_route_id,
      main_frame_widget_route_id, cloned_namespace->id());
}
