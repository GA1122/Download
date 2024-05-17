void RenderFrameHostImpl::DidCommitProvisionalLoad(
    std::unique_ptr<FrameHostMsg_DidCommitProvisionalLoad_Params>
        validated_params,
    service_manager::mojom::InterfaceProviderRequest
        interface_provider_request) {
  ScopedActiveURL scoped_active_url(
      validated_params->url,
      frame_tree_node()->frame_tree()->root()->current_origin());

  ScopedCommitStateResetter commit_state_resetter(this);
  RenderProcessHost* process = GetProcess();

  TRACE_EVENT2("navigation", "RenderFrameHostImpl::DidCommitProvisionalLoad",
               "frame_tree_node", frame_tree_node_->frame_tree_node_id(), "url",
               validated_params->url.possibly_invalid_spec());

  DCHECK_EQ(ui::PageTransitionIsMainFrame(validated_params->transition),
            !GetParent());

  NotifyResourceSchedulerOfNavigation(process->GetID(), *validated_params);

  if (is_waiting_for_beforeunload_ack_ && unload_ack_is_for_navigation_ &&
      !GetParent()) {
    base::TimeTicks approx_renderer_start_time = send_before_unload_start_time_;
    OnBeforeUnloadACK(true, approx_renderer_start_time, base::TimeTicks::Now());
  }

  if (IsWaitingForUnloadACK())
    return;

  DCHECK(document_scoped_interface_provider_binding_.is_bound());

  if (validated_params->was_within_same_document) {
    if (interface_provider_request.is_pending()) {
      bad_message::ReceivedBadMessage(
          process, bad_message::RFH_INTERFACE_PROVIDER_SUPERFLUOUS);
      return;
    }
  } else if (interface_provider_request.is_pending()) {
    document_scoped_interface_provider_binding_.Close();
    BindInterfaceProviderRequest(std::move(interface_provider_request));
  } else {
    if (frame_tree_node_->has_committed_real_load()) {
      document_scoped_interface_provider_binding_.Close();
      bad_message::ReceivedBadMessage(
          process, bad_message::RFH_INTERFACE_PROVIDER_MISSING);
      return;
    }

  }

  if (validated_params->report_type ==
      FrameMsg_UILoadMetricsReportType::REPORT_LINK) {
    UMA_HISTOGRAM_CUSTOM_TIMES(
        "Navigation.UI_OnCommitProvisionalLoad.Link",
        base::TimeTicks::Now() - validated_params->ui_timestamp,
        base::TimeDelta::FromMilliseconds(10), base::TimeDelta::FromMinutes(10),
        100);
  } else if (validated_params->report_type ==
             FrameMsg_UILoadMetricsReportType::REPORT_INTENT) {
    UMA_HISTOGRAM_CUSTOM_TIMES(
        "Navigation.UI_OnCommitProvisionalLoad.Intent",
        base::TimeTicks::Now() - validated_params->ui_timestamp,
        base::TimeDelta::FromMilliseconds(10), base::TimeDelta::FromMinutes(10),
        100);
  }

  if (!CanCommitURL(validated_params->url)) {
    VLOG(1) << "Blocked URL " << validated_params->url.spec();
    bad_message::ReceivedBadMessage(process,
                                    bad_message::RFH_CAN_COMMIT_URL_BLOCKED);
    return;
  }

  if (!CanCommitOrigin(validated_params->origin, validated_params->url)) {
    DEBUG_ALIAS_FOR_ORIGIN(origin_debug_alias, validated_params->origin);
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_INVALID_ORIGIN_ON_COMMIT);
    return;
  }

  process->FilterURL(false, &validated_params->url);
  process->FilterURL(true, &validated_params->referrer.url);
  for (std::vector<GURL>::iterator it(validated_params->redirects.begin());
       it != validated_params->redirects.end(); ++it) {
    process->FilterURL(false, &(*it));
  }
  process->FilterURL(true, &validated_params->searchable_form_url);

  if (!CanAccessFilesOfPageState(validated_params->page_state)) {
    bad_message::ReceivedBadMessage(
        GetProcess(), bad_message::RFH_CAN_ACCESS_FILES_OF_PAGE_STATE);
    return;
  }

  if (!navigation_request_) {
    if (!is_loading()) {
      bool was_loading = frame_tree_node()->frame_tree()->IsLoading();
      is_loading_ = true;
      frame_tree_node()->DidStartLoading(true, was_loading);
    }
    pending_commit_ = false;
  }

  std::unique_ptr<NavigationHandleImpl> navigation_handle =
      TakeNavigationHandleForCommit(*validated_params);
  DCHECK(navigation_handle);

  if (validated_params->url_is_unreachable ||
      delegate_->GetAsInterstitialPage()) {
    SetLastCommittedSiteUrl(GURL());
  } else {
    SetLastCommittedSiteUrl(validated_params->url);
  }

  accessibility_reset_count_ = 0;
  frame_tree_node()->navigator()->DidNavigate(this, *validated_params,
                                              std::move(navigation_handle));

  commit_state_resetter.disable();

  if (frame_tree_node_->IsMainFrame() && GetView() &&
      !validated_params->was_within_same_document) {
    RenderWidgetHostImpl::From(GetView()->GetRenderWidgetHost())
        ->DidNavigate(validated_params->content_source_id);
  }
}
