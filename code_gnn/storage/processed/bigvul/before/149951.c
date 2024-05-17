EventListenerProperties LayerTreeHostImpl::GetEventListenerProperties(
    EventListenerClass event_class) const {
  return active_tree_->event_listener_properties(event_class);
}
