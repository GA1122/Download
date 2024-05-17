void ReceiverConnectionProxy::Bind(
    blink::mojom::PresentationConnectionRequest receiver_connection_request) {
  binding_.Bind(std::move(receiver_connection_request));
}
