FrameDeletedObserver::FrameDeletedObserver(RenderFrameHost* owner_host)
    : impl_(new FrameTreeNodeObserverImpl(
          static_cast<RenderFrameHostImpl*>(owner_host)->frame_tree_node())) {}
