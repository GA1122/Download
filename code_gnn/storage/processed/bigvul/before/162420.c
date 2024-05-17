MojoResult Core::ReadMessage(MojoHandle message_pipe_handle,
                             MojoMessageHandle* message_handle,
                             MojoReadMessageFlags flags) {
  RequestContext request_context;
  auto dispatcher = GetDispatcher(message_pipe_handle);
  if (!dispatcher || !message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;

  std::unique_ptr<ports::UserMessageEvent> message_event;
  MojoResult rv = dispatcher->ReadMessage(&message_event);
  if (rv != MOJO_RESULT_OK)
    return rv;

  *message_handle =
      reinterpret_cast<MojoMessageHandle>(message_event.release());
  return MOJO_RESULT_OK;
}
