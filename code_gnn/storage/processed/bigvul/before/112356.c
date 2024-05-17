void OnSwapOutACKHelper(int render_process_id, int render_view_id) {
  RenderViewHostImpl* rvh = RenderViewHostImpl::FromID(render_process_id,
                                                       render_view_id);
  if (rvh)
    rvh->OnSwapOutACK();
}
