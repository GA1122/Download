  void OnReceivedRoutingIDOnUI(int widget_routing_id) {
    routing_id_ = widget_routing_id;
    message_loop_runner_->Quit();
  }
