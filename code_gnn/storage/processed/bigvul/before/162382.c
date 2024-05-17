MojoResult Core::AttachSerializedMessageBuffer(MojoMessageHandle message_handle,
                                               uint32_t payload_size,
                                               const MojoHandle* handles,
                                               uint32_t num_handles,
                                               void** buffer,
                                               uint32_t* buffer_size) {
  if (!message_handle || (num_handles && !handles) || !buffer || !buffer_size)
    return MOJO_RESULT_INVALID_ARGUMENT;
  RequestContext request_context;
  auto* message = reinterpret_cast<ports::UserMessageEvent*>(message_handle)
                      ->GetMessage<UserMessageImpl>();
  MojoResult rv = message->AttachSerializedMessageBuffer(payload_size, handles,
                                                         num_handles);
  if (rv != MOJO_RESULT_OK)
    return rv;

  *buffer = message->user_payload();
  *buffer_size = base::checked_cast<uint32_t>(message->user_payload_capacity());
  return MOJO_RESULT_OK;
}
