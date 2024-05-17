void RenderFrameHostManager::CommitPendingFramePolicy() {
  if (!frame_tree_node_->CommitPendingFramePolicy())
    return;

  CHECK(frame_tree_node_->parent());

  DCHECK(!frame_tree_node_->child_count());

  SiteInstance* parent_site_instance =
      frame_tree_node_->parent()->current_frame_host()->GetSiteInstance();
  for (const auto& pair : proxy_hosts_) {
    if (pair.second->GetSiteInstance() != parent_site_instance) {
      pair.second->Send(new FrameMsg_DidUpdateFramePolicy(
          pair.second->GetRoutingID(),
          frame_tree_node_->current_replication_state().frame_policy));
    }
  }
}
