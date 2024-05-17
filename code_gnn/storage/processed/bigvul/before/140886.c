void PresentationConnectionProxy::SendConnectionMessage(
    PresentationConnectionMessage message,
    const OnMessageCallback& callback) const {
  DCHECK(target_connection_ptr_);
  target_connection_ptr_->OnMessage(std::move(message), callback);
}
