void NavigationRequest::UpdateRequestNavigationParamsHistory() {
  NavigationController* navigation_controller =
      frame_tree_node_->navigator()->GetController();
  request_params_.current_history_list_offset =
      navigation_controller->GetCurrentEntryIndex();
  request_params_.current_history_list_length =
      navigation_controller->GetEntryCount();
}
