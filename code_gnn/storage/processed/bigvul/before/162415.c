MojoResult Core::NotifyBadMessage(MojoMessageHandle message_handle,
                                  const char* error,
                                  size_t error_num_bytes) {
  if (!message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;

  auto* message_event =
      reinterpret_cast<ports::UserMessageEvent*>(message_handle);
  auto* message = message_event->GetMessage<UserMessageImpl>();
  if (message->source_node() == ports::kInvalidNodeName) {
    DVLOG(1) << "Received invalid message from unknown node.";
    if (!default_process_error_callback_.is_null())
      default_process_error_callback_.Run(std::string(error, error_num_bytes));
    return MOJO_RESULT_OK;
  }

  GetNodeController()->NotifyBadMessageFrom(
      message->source_node(), std::string(error, error_num_bytes));
  return MOJO_RESULT_OK;
}
