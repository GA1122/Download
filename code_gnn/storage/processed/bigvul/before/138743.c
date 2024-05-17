void RenderFrameHostImpl::OnDidChangeName(const std::string& name,
                                          const std::string& unique_name) {
  if (GetParent() != nullptr) {
    DCHECK(!unique_name.empty());
  }

  std::string old_name = frame_tree_node()->frame_name();
  frame_tree_node()->SetFrameName(name, unique_name);
  if (old_name.empty() && !name.empty())
    frame_tree_node_->render_manager()->CreateProxiesForNewNamedFrame();
  delegate_->DidChangeName(this, name);
}
