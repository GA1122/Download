void RenderFrameHostImpl::PostMessageEvent(int32_t source_routing_id,
                                           const base::string16& source_origin,
                                           const base::string16& target_origin,
                                           blink::TransferableMessage message) {
  GetNavigationControl()->PostMessageEvent(source_routing_id, source_origin,
                                           target_origin, std::move(message));
}
