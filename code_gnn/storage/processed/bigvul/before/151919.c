void RenderFrameHostImpl::DidCommitNavigation(
    std::unique_ptr<NavigationRequest> committing_navigation_request,
    std::unique_ptr<FrameHostMsg_DidCommitProvisionalLoad_Params>
        validated_params,
    mojom::DidCommitProvisionalLoadInterfaceParamsPtr interface_params) {
  NavigationHandleImpl* navigation_handle;
  if (committing_navigation_request) {
    navigation_handle = committing_navigation_request->navigation_handle();
  } else {
    navigation_handle = GetNavigationHandle();
  }

  if (navigation_handle) {
    main_frame_request_ids_ = {validated_params->request_id,
                               navigation_handle->GetGlobalRequestID()};
    if (deferred_main_frame_load_info_)
      ResourceLoadComplete(std::move(deferred_main_frame_load_info_));
  }
  ScopedActiveURL scoped_active_url(
      validated_params->url,
      frame_tree_node()->frame_tree()->root()->current_origin());

  ScopedCommitStateResetter commit_state_resetter(this);
  RenderProcessHost* process = GetProcess();

  TRACE_EVENT2("navigation", "RenderFrameHostImpl::DidCommitProvisionalLoad",
               "url", validated_params->url.possibly_invalid_spec(), "details",
               CommitAsTracedValue(validated_params.get()));

  if (is_waiting_for_beforeunload_ack_ && unload_ack_is_for_navigation_ &&
      !GetParent()) {
    base::TimeTicks approx_renderer_start_time = send_before_unload_start_time_;
    ProcessBeforeUnloadACK(true  , true  ,
                           approx_renderer_start_time, base::TimeTicks::Now());
  }

  if (!is_active())
    return;

  DCHECK(document_scoped_interface_provider_binding_.is_bound());
  if (interface_params) {
    auto interface_provider_request_of_previous_document =
        document_scoped_interface_provider_binding_.Unbind();
    dropped_interface_request_logger_ =
        std::make_unique<DroppedInterfaceRequestLogger>(
            std::move(interface_provider_request_of_previous_document));
    BindInterfaceProviderRequest(
        std::move(interface_params->interface_provider_request));

    document_interface_broker_content_binding_.Close();
    document_interface_broker_blink_binding_.Close();
    BindDocumentInterfaceBrokerRequest(
        std::move(interface_params->document_interface_broker_content_request),
        std::move(interface_params->document_interface_broker_blink_request));

  } else {
    if (frame_tree_node_->has_committed_real_load()) {
      document_scoped_interface_provider_binding_.Close();
      document_interface_broker_content_binding_.Close();
      document_interface_broker_blink_binding_.Close();
      bad_message::ReceivedBadMessage(
          process, bad_message::RFH_INTERFACE_PROVIDER_MISSING);
      return;
    }

  }

  if (!DidCommitNavigationInternal(std::move(committing_navigation_request),
                                   validated_params.get(),
                                   false  )) {
    return;
  }

  commit_state_resetter.disable();

  if (frame_tree_node_->IsMainFrame() && GetView()) {
    RenderWidgetHostImpl::From(GetView()->GetRenderWidgetHost())
        ->DidNavigate(validated_params->content_source_id);
  }
}
