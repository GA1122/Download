void ReceiverConnectionProxy::BindControllerConnection(
    blink::mojom::PresentationConnectionPtr controller_connection_ptr) {
  DCHECK(!target_connection_ptr_);
  target_connection_ptr_ = std::move(controller_connection_ptr);
  target_connection_ptr_->DidChangeState(
      content::PRESENTATION_CONNECTION_STATE_CONNECTED);

  DidChangeState(content::PRESENTATION_CONNECTION_STATE_CONNECTED);
}
