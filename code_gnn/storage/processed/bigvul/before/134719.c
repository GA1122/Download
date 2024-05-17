void GuestViewBase::DispatchEventToGuestProxy(GuestViewEvent* event) {
  event->Dispatch(this, guest_instance_id_);
}
