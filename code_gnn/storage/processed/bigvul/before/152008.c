void NotifyForEachFrameFromUI(RenderFrameHostImpl* root_frame_host,
                              const FrameNotifyCallback& frame_callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  FrameTree* frame_tree = root_frame_host->frame_tree_node()->frame_tree();
  DCHECK_EQ(root_frame_host, frame_tree->GetMainFrame());

  auto routing_ids = std::make_unique<std::set<GlobalFrameRoutingId>>();
  for (FrameTreeNode* node : frame_tree->Nodes()) {
    RenderFrameHostImpl* frame_host = node->current_frame_host();
    RenderFrameHostImpl* pending_frame_host =
        node->render_manager()->speculative_frame_host();
    if (frame_host)
      routing_ids->insert(frame_host->GetGlobalFrameRoutingId());
    if (pending_frame_host)
      routing_ids->insert(pending_frame_host->GetGlobalFrameRoutingId());
  }
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::IO},
      base::BindOnce(&NotifyRouteChangesOnIO, frame_callback,
                     std::move(routing_ids)));
}
