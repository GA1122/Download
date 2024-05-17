void NotifyForEachFrameFromUI(RenderFrameHost* root_frame_host,
                              const FrameCallback& frame_callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  FrameTree* frame_tree = static_cast<RenderFrameHostImpl*>(root_frame_host)
                              ->frame_tree_node()
                              ->frame_tree();
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
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::BindOnce(&NotifyRouteChangesOnIO, frame_callback,
                     std::move(routing_ids)));
}
