MojoResult Core::ExtendSerializedMessagePayload(
    MojoMessageHandle message_handle,
    uint32_t new_payload_size,
    const MojoHandle* handles,
    uint32_t num_handles,
    void** new_buffer,
    uint32_t* new_buffer_size) {
  if (!message_handle || !new_buffer || !new_buffer_size)
    return MOJO_RESULT_INVALID_ARGUMENT;
  if (!handles && num_handles)
    return MOJO_RESULT_INVALID_ARGUMENT;
  auto* message = reinterpret_cast<ports::UserMessageEvent*>(message_handle)
                      ->GetMessage<UserMessageImpl>();
  MojoResult rv = message->ExtendSerializedMessagePayload(new_payload_size,
                                                          handles, num_handles);
  if (rv != MOJO_RESULT_OK)
    return rv;

  *new_buffer = message->user_payload();
  *new_buffer_size =
      base::checked_cast<uint32_t>(message->user_payload_capacity());
  return MOJO_RESULT_OK;
}
