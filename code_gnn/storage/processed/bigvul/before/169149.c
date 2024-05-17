void RenderFrameHostImpl::CreateWebSocket(
    network::mojom::WebSocketRequest request) {
  WebSocketManager::CreateWebSocket(process_->GetID(), routing_id_,
                                    last_committed_origin_, std::move(request));
}
