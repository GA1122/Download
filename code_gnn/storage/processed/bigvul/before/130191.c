void RenderFrameHostImpl::OnNavigate(const IPC::Message& msg) {
  PickleIterator iter(msg);
  FrameHostMsg_DidCommitProvisionalLoad_Params validated_params;
  if (!IPC::ParamTraits<FrameHostMsg_DidCommitProvisionalLoad_Params>::
      Read(&msg, &iter, &validated_params))
    return;

  if (render_view_host_->is_waiting_for_beforeunload_ack_ &&
      render_view_host_->unload_ack_is_for_cross_site_transition_ &&
      PageTransitionIsMainFrame(validated_params.transition)) {
    OnBeforeUnloadACK(true, send_before_unload_start_time_,
                      base::TimeTicks::Now());
    return;
  }

  if (render_view_host_->IsWaitingForUnloadACK())
    return;

  RenderProcessHost* process = GetProcess();

  if (!CanCommitURL(validated_params.url)) {
    VLOG(1) << "Blocked URL " << validated_params.url.spec();
    validated_params.url = GURL(kAboutBlankURL);
    RecordAction(base::UserMetricsAction("CanCommitURL_BlockedAndKilled"));
    process->ReceivedBadMessage();
  }

  render_view_host_->has_accessed_initial_document_ = false;

  process->FilterURL(false, &validated_params.url);
  process->FilterURL(true, &validated_params.referrer.url);
  for (std::vector<GURL>::iterator it(validated_params.redirects.begin());
      it != validated_params.redirects.end(); ++it) {
    process->FilterURL(false, &(*it));
  }
  process->FilterURL(true, &validated_params.searchable_form_url);

  if (!render_view_host_->CanAccessFilesOfPageState(
          validated_params.page_state)) {
    GetProcess()->ReceivedBadMessage();
    return;
  }

  frame_tree_node()->navigator()->DidNavigate(this, validated_params);
}
