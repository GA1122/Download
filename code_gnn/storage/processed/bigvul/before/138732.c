void RenderFrameHostImpl::OnBeforeUnloadACK(
    bool proceed,
    const base::TimeTicks& renderer_before_unload_start_time,
    const base::TimeTicks& renderer_before_unload_end_time) {
  TRACE_EVENT_ASYNC_END1("navigation", "RenderFrameHostImpl BeforeUnload", this,
                         "FrameTreeNode id",
                         frame_tree_node_->frame_tree_node_id());
  if (!is_waiting_for_beforeunload_ack_) {
    return;
  }
  DCHECK(!send_before_unload_start_time_.is_null());

  base::TimeTicks before_unload_end_time = renderer_before_unload_end_time;
  if (!renderer_before_unload_start_time.is_null() &&
      !renderer_before_unload_end_time.is_null()) {
    base::TimeTicks receive_before_unload_ack_time = base::TimeTicks::Now();

    if (!base::TimeTicks::IsConsistentAcrossProcesses()) {
      InterProcessTimeTicksConverter converter(
          LocalTimeTicks::FromTimeTicks(send_before_unload_start_time_),
          LocalTimeTicks::FromTimeTicks(receive_before_unload_ack_time),
          RemoteTimeTicks::FromTimeTicks(renderer_before_unload_start_time),
          RemoteTimeTicks::FromTimeTicks(renderer_before_unload_end_time));
      LocalTimeTicks browser_before_unload_end_time =
          converter.ToLocalTimeTicks(
              RemoteTimeTicks::FromTimeTicks(renderer_before_unload_end_time));
      before_unload_end_time = browser_before_unload_end_time.ToTimeTicks();

      bool is_skew_additive = false;
      if (converter.IsSkewAdditiveForMetrics()) {
        is_skew_additive = true;
        base::TimeDelta skew = converter.GetSkewForMetrics();
        if (skew >= base::TimeDelta()) {
          UMA_HISTOGRAM_TIMES(
              "InterProcessTimeTicks.BrowserBehind_RendererToBrowser", skew);
        } else {
          UMA_HISTOGRAM_TIMES(
              "InterProcessTimeTicks.BrowserAhead_RendererToBrowser", -skew);
        }
      }
      UMA_HISTOGRAM_BOOLEAN(
          "InterProcessTimeTicks.IsSkewAdditive_RendererToBrowser",
          is_skew_additive);
    }

    base::TimeDelta on_before_unload_overhead_time =
        (receive_before_unload_ack_time - send_before_unload_start_time_) -
        (renderer_before_unload_end_time - renderer_before_unload_start_time);
    UMA_HISTOGRAM_TIMES("Navigation.OnBeforeUnloadOverheadTime",
                        on_before_unload_overhead_time);

    frame_tree_node_->navigator()->LogBeforeUnloadTime(
        renderer_before_unload_start_time, renderer_before_unload_end_time);
  }
  is_waiting_for_beforeunload_ack_ = false;
  if (beforeunload_timeout_)
    beforeunload_timeout_->Stop();
  send_before_unload_start_time_ = base::TimeTicks();

  if (IsBrowserSideNavigationEnabled() && unload_ack_is_for_navigation_) {
    frame_tree_node_->navigator()->OnBeforeUnloadACK(
        frame_tree_node_, proceed);
  } else {
    frame_tree_node_->render_manager()->OnBeforeUnloadACK(
        unload_ack_is_for_navigation_, proceed,
        before_unload_end_time);
  }

  if (!proceed)
    render_view_host_->GetDelegate()->DidCancelLoading();
}
