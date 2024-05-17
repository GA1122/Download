MojoResult Core::GetSerializedMessageContents(
    MojoMessageHandle message_handle,
    void** buffer,
    uint32_t* num_bytes,
    MojoHandle* handles,
    uint32_t* num_handles,
    MojoGetSerializedMessageContentsFlags flags) {
  if (!message_handle || (num_handles && *num_handles && !handles))
    return MOJO_RESULT_INVALID_ARGUMENT;

  auto* message = reinterpret_cast<ports::UserMessageEvent*>(message_handle)
                      ->GetMessage<UserMessageImpl>();
  if (!message->IsSerialized() || !message->IsTransmittable())
    return MOJO_RESULT_FAILED_PRECONDITION;

  if (num_bytes) {
    base::CheckedNumeric<uint32_t> payload_size = message->user_payload_size();
    *num_bytes = payload_size.ValueOrDie();
  }

  if (message->user_payload_size() > 0) {
    if (!num_bytes || !buffer)
      return MOJO_RESULT_RESOURCE_EXHAUSTED;

    *buffer = message->user_payload();
  } else if (buffer) {
    *buffer = nullptr;
  }

  uint32_t max_num_handles = 0;
  if (num_handles) {
    max_num_handles = *num_handles;
    *num_handles = static_cast<uint32_t>(message->num_handles());
  }

  if (message->num_handles() > max_num_handles ||
      message->num_handles() > kMaxHandlesPerMessage) {
    return MOJO_RESULT_RESOURCE_EXHAUSTED;
  }

  RequestContext request_context;
  return message->ExtractSerializedHandles(
      UserMessageImpl::ExtractBadHandlePolicy::kAbort, handles);
}
