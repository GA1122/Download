void GuestViewBase::SendQueuedEvents() {
  if (!attached())
    return;
  while (!pending_events_.empty()) {
    linked_ptr<GuestViewEvent> event_ptr = pending_events_.front();
    pending_events_.pop_front();
    event_ptr.release()->Dispatch(this, view_instance_id_);
  }
}
