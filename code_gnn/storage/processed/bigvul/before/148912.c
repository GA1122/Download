void RenderFrameHostManager::SetRWHViewForInnerContents(
    RenderWidgetHostView* child_rwhv) {
  DCHECK(ForInnerDelegate() && frame_tree_node_->IsMainFrame());
  GetProxyToOuterDelegate()->SetChildRWHView(child_rwhv);
}
