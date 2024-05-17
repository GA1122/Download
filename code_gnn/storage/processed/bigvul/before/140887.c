void PresentationConnectionProxy::close() const {
  DCHECK(target_connection_ptr_);
  target_connection_ptr_->OnClose();
}
