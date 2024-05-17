void RenderFrameHostManager::OnDidSetFramePolicyHeaders() {
  for (const auto& pair : proxy_hosts_) {
    pair.second->Send(new FrameMsg_DidSetFramePolicyHeaders(
        pair.second->GetRoutingID(), frame_tree_node_->active_sandbox_flags(),
        frame_tree_node_->current_replication_state().feature_policy_header));
  }
}
