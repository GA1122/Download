void RenderViewHostImpl::OnClosePageACK() {
  decrement_in_flight_event_count();
  ClosePageIgnoringUnloadEvents();
}
