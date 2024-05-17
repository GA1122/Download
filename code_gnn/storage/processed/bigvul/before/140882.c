ControllerConnectionProxy::MakeRemoteRequest() {
  DCHECK(!target_connection_ptr_)
      << "target_connection_ptr_ should only be bound once.";
  return mojo::MakeRequest(&target_connection_ptr_);
}
