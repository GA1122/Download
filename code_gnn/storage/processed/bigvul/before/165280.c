void NavigationRequest::UpdateCommitNavigationParamsHistory() {
  NavigationController* navigation_controller =
      frame_tree_node_->navigator()->GetController();
  commit_params_.current_history_list_offset =
      navigation_controller->GetCurrentEntryIndex();
  commit_params_.current_history_list_length =
      navigation_controller->GetEntryCount();
}
