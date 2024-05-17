void RenderFrameHostImpl::CreateWebSocket(
    network::mojom::WebSocketRequest request) {
  network::mojom::AuthenticationHandlerPtr auth_handler;

  network::mojom::TrustedHeaderClientPtr header_client;
  uint32_t options = network::mojom::kWebSocketOptionNone;
  GetContentClient()->browser()->WillCreateWebSocket(
      this, &request, &auth_handler, &header_client, &options);

  WebSocketManager::CreateWebSocket(
      process_->GetID(), routing_id_, last_committed_origin_, options,
      std::move(auth_handler), std::move(header_client), std::move(request));
}
