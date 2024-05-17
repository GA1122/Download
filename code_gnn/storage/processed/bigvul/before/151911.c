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

  bool effective_transient_activation_state =
      params->mimic_user_gesture ||
      (base::FeatureList::IsEnabled(features::kUserActivationV2) &&
       frame_tree_node_->HasTransientUserActivation());

  bool can_create_window =
      IsCurrent() && render_frame_created_ &&
      GetContentClient()->browser()->CanCreateWindow(
          this, GetLastCommittedURL(),
          frame_tree_node_->frame_tree()->GetMainFrame()->GetLastCommittedURL(),
          last_committed_origin_, params->window_container_type,
          params->target_url, params->referrer.To<Referrer>(),
          params->frame_name, params->disposition, *params->features,
          effective_transient_activation_state, params->opener_suppressed,
          &no_javascript_access);

  bool was_consumed = false;
  if (can_create_window) {
    was_consumed = frame_tree_node_->UpdateUserActivationState(
        blink::UserActivationUpdateType::kConsumeTransientActivation);
  }

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
  if (!params->clone_from_session_storage_namespace_id.empty()) {
    cloned_namespace = SessionStorageNamespaceImpl::CloneFrom(
        dom_storage_context, params->session_storage_namespace_id,
        params->clone_from_session_storage_namespace_id);
  } else {
    cloned_namespace = SessionStorageNamespaceImpl::Create(
        dom_storage_context, params->session_storage_namespace_id);
  }

  int render_view_route_id = MSG_ROUTING_NONE;
  int main_frame_route_id = MSG_ROUTING_NONE;
  int main_frame_widget_route_id = MSG_ROUTING_NONE;
  int render_process_id = GetProcess()->GetID();
  if (!params->opener_suppressed && !no_javascript_access) {
    render_view_route_id = GetProcess()->GetNextRoutingID();
    main_frame_route_id = GetProcess()->GetNextRoutingID();
    main_frame_widget_route_id = GetProcess()->GetNextRoutingID();
    if (!base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      auto block_requests_for_route = base::Bind(
          [](const GlobalFrameRoutingId& id) {
            auto* rdh = ResourceDispatcherHostImpl::Get();
            if (rdh)
              rdh->BlockRequestsForRoute(id);
          },
          GlobalFrameRoutingId(render_process_id, main_frame_route_id));
      base::PostTaskWithTraits(FROM_HERE, {BrowserThread::IO},
                               std::move(block_requests_for_route));
    }
  }

  DCHECK(IsRenderFrameLive());

  bool opened_by_user_activation = params->mimic_user_gesture;
  if (base::FeatureList::IsEnabled(features::kUserActivationV2))
    opened_by_user_activation = was_consumed;

  delegate_->CreateNewWindow(this, render_view_route_id, main_frame_route_id,
                             main_frame_widget_route_id, *params,
                             opened_by_user_activation, cloned_namespace.get());

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

  rfh->SetOriginOfNewFrame(GetLastCommittedOrigin());

  if (base::FeatureList::IsEnabled(network::features::kNetworkService) &&
      rfh->waiting_for_init_) {
    rfh->frame_->BlockRequests();
  }

  service_manager::mojom::InterfaceProviderPtrInfo
      main_frame_interface_provider_info;
  rfh->BindInterfaceProviderRequest(
      mojo::MakeRequest(&main_frame_interface_provider_info));

  blink::mojom::DocumentInterfaceBrokerPtrInfo
      document_interface_broker_content_info;

  blink::mojom::DocumentInterfaceBrokerPtrInfo
      document_interface_broker_blink_info;
  rfh->BindDocumentInterfaceBrokerRequest(
      mojo::MakeRequest(&document_interface_broker_content_info),
      mojo::MakeRequest(&document_interface_broker_blink_info));

  mojom::CreateNewWindowReplyPtr reply = mojom::CreateNewWindowReply::New(
      render_view_route_id, main_frame_route_id, main_frame_widget_route_id,
      mojom::DocumentScopedInterfaceBundle::New(
          std::move(main_frame_interface_provider_info),
          std::move(document_interface_broker_content_info),
          std::move(document_interface_broker_blink_info)),
      cloned_namespace->id(), rfh->GetDevToolsFrameToken());
  std::move(callback).Run(mojom::CreateNewWindowStatus::kSuccess,
                          std::move(reply));
}
