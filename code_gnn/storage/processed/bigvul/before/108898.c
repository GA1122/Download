void RenderViewImpl::GuestReady(PP_Instance instance) {
  guest_pp_instance_ = instance;
  if (guest_uninitialized_context_) {
    bool success = GetGuestToEmbedderChannel()->CreateGraphicsContext(
        guest_uninitialized_context_,
        guest_attributes_,
        false,
        this);
    DCHECK(success);
    CompleteInit(host_window_);
    guest_uninitialized_context_ = NULL;
  }
}
