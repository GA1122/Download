void DevToolsSession::DispatchProtocolMessage(
    blink::mojom::DevToolsMessageChunkPtr chunk) {
  if (chunk->is_first && !response_message_buffer_.empty()) {
    ReceivedBadMessage();
    return;
  }

  response_message_buffer_ += std::move(chunk->data);

  if (!chunk->is_last)
    return;

  if (!chunk->post_state.empty())
    state_cookie_ = std::move(chunk->post_state);
  waiting_for_response_messages_.erase(chunk->call_id);
  std::string message;
  message.swap(response_message_buffer_);
  client_->DispatchProtocolMessage(agent_host_, message);
}
