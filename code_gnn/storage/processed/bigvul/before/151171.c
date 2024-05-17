void InspectorNetworkAgent::DidReceiveWebSocketFrame(unsigned long identifier,
                                                     int op_code,
                                                     bool masked,
                                                     const char* payload,
                                                     size_t payload_length) {
  std::unique_ptr<protocol::Network::WebSocketFrame> frame_object =
      protocol::Network::WebSocketFrame::create()
          .setOpcode(op_code)
          .setMask(masked)
          .setPayloadData(
              String::FromUTF8WithLatin1Fallback(payload, payload_length))
          .build();
  GetFrontend()->webSocketFrameReceived(
      IdentifiersFactory::RequestId(identifier), MonotonicallyIncreasingTime(),
      std::move(frame_object));
}
