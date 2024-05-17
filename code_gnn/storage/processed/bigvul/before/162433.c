MojoResult Core::WriteMessage(MojoHandle message_pipe_handle,
                              MojoMessageHandle message_handle,
                              MojoWriteMessageFlags flags) {
  RequestContext request_context;
  if (!message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;
  auto message_event = base::WrapUnique(
      reinterpret_cast<ports::UserMessageEvent*>(message_handle));
  auto* message = message_event->GetMessage<UserMessageImpl>();
  if (!message || !message->IsTransmittable())
    return MOJO_RESULT_INVALID_ARGUMENT;
  auto dispatcher = GetDispatcher(message_pipe_handle);
  if (!dispatcher)
    return MOJO_RESULT_INVALID_ARGUMENT;
  return dispatcher->WriteMessage(std::move(message_event), flags);
}
