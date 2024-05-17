void GuestViewBase::DispatchEventToView(GuestViewEvent* event) {
  if (!attached() &&
      (!CanRunInDetachedState() || !can_owner_receive_events())) {
    pending_events_.push_back(linked_ptr<GuestViewEvent>(event));
    return;
  }

  event->Dispatch(this, view_instance_id_);
}
