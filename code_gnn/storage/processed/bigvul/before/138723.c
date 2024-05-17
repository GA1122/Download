void NotifyForEachFrameFromUI(
    RenderFrameHost* root_frame_host,
    base::Callback<void(ResourceDispatcherHostImpl*,
                        const GlobalFrameRoutingId&)> frame_callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  FrameTree* frame_tree = static_cast<RenderFrameHostImpl*>(root_frame_host)
                              ->frame_tree_node()
                              ->frame_tree();
  DCHECK_EQ(root_frame_host, frame_tree->GetMainFrame());

  std::unique_ptr<std::set<GlobalFrameRoutingId>> routing_ids(
      new std::set<GlobalFrameRoutingId>());
  for (FrameTreeNode* node : frame_tree->Nodes()) {
    RenderFrameHostImpl* frame_host = node->current_frame_host();
    RenderFrameHostImpl* pending_frame_host =
        IsBrowserSideNavigationEnabled()
            ? node->render_manager()->speculative_frame_host()
            : node->render_manager()->pending_frame_host();
    if (frame_host)
      routing_ids->insert(frame_host->GetGlobalFrameRoutingId());
    if (pending_frame_host)
      routing_ids->insert(pending_frame_host->GetGlobalFrameRoutingId());
  }
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::Bind(&NotifyRouteChangesOnIO, frame_callback,
                                     base::Passed(std::move(routing_ids))));
}
