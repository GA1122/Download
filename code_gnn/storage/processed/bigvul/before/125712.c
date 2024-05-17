void RenderViewHostImpl::SetSwappedOut(bool is_swapped_out) {
  is_swapped_out_ = is_swapped_out;

  is_waiting_for_beforeunload_ack_ = false;
  is_waiting_for_unload_ack_ = false;
  has_timed_out_on_unload_ = false;
}
