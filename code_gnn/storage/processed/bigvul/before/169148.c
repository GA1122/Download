void RenderFrameHostImpl::CreateNewWindow(
    mojom::CreateNewWindowParamsPtr params,
    CreateNewWindowCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  TRACE_EVENT2("navigation", "RenderFrameHostImpl::CreateNewWindow",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(), "url",
               params->target_url.possibly_invalid_spec());

  bool no_javascript_access = false;

  GetProcess()->FilterURL(false, &params->target_url);
  if (!GetContentClient()->browser()->ShouldAllowOpenURL(GetSiteInstance(),
                                                         params->target_url)) {
    params->target_url = GURL(url::kAboutBlankURL);
  }

  bool can_create_window =
      IsCurrent() && render_frame_created_ &&
      GetContentClient()->browser()->CanCreateWindow(
          this, GetLastCommittedURL(),
          frame_tree_node_->frame_tree()->GetMainFrame()->GetLastCommittedURL(),
          last_committed_origin_.GetURL(), params->window_container_type,
          params->target_url, params->referrer, params->frame_name,
          params->disposition, *params->features, params->user_gesture,
          params->opener_suppressed, &no_javascript_access);

  if (!can_create_window) {
    std::move(callback).Run(mojom::CreateNewWindowStatus::kIgnore, nullptr);
    return;
  }

  if (!render_view_host_->GetWebkitPreferences().supports_multiple_windows) {
    std::move(callback).Run(mojom::CreateNewWindowStatus::kReuse, nullptr);
    return;
  }

  StoragePartition* storage_partition = BrowserContext::GetStoragePartition(
      GetSiteInstance()->GetBrowserContext(), GetSiteInstance());
  DOMStorageContextWrapper* dom_storage_context =
      static_cast<DOMStorageContextWrapper*>(
          storage_partition->GetDOMStorageContext());

  scoped_refptr<SessionStorageNamespaceImpl> cloned_namespace;
  if (base::FeatureList::IsEnabled(features::kMojoSessionStorage)) {
    cloned_namespace = SessionStorageNamespaceImpl::Create(
        dom_storage_context, params->session_storage_namespace_id);
  } else {
    cloned_namespace = SessionStorageNamespaceImpl::CloneFrom(
        dom_storage_context, params->session_storage_namespace_id,
        params->clone_from_session_storage_namespace_id);
  }

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
                            std::move(block_requests_for_route));
  }

  DCHECK(IsRenderFrameLive());

  delegate_->CreateNewWindow(this, render_view_route_id, main_frame_route_id,
                             main_frame_widget_route_id, *params,
                             cloned_namespace.get());

  if (main_frame_route_id == MSG_ROUTING_NONE) {
    std::move(callback).Run(mojom::CreateNewWindowStatus::kIgnore, nullptr);
    return;
  }

  bool succeeded =
      RenderWidgetHost::FromID(render_process_id, main_frame_widget_route_id) !=
      nullptr;
  if (!succeeded) {
    DCHECK(!RenderFrameHost::FromID(render_process_id, main_frame_route_id));
    DCHECK(!RenderViewHost::FromID(render_process_id, render_view_route_id));
    std::move(callback).Run(mojom::CreateNewWindowStatus::kIgnore, nullptr);
    return;
  }

  DCHECK(RenderViewHost::FromID(render_process_id, render_view_route_id));
  RenderFrameHostImpl* rfh =
      RenderFrameHostImpl::FromID(GetProcess()->GetID(), main_frame_route_id);
  DCHECK(rfh);

  service_manager::mojom::InterfaceProviderPtrInfo
      main_frame_interface_provider_info;
  rfh->BindInterfaceProviderRequest(
      mojo::MakeRequest(&main_frame_interface_provider_info));

  mojom::CreateNewWindowReplyPtr reply = mojom::CreateNewWindowReply::New(
      render_view_route_id, main_frame_route_id, main_frame_widget_route_id,
      std::move(main_frame_interface_provider_info), cloned_namespace->id(),
      rfh->GetDevToolsFrameToken());
  std::move(callback).Run(mojom::CreateNewWindowStatus::kSuccess,
                          std::move(reply));
}
