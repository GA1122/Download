MojoResult Core::SerializeMessage(MojoMessageHandle message_handle) {
  if (!message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;
  RequestContext request_context;
  return reinterpret_cast<ports::UserMessageEvent*>(message_handle)
      ->GetMessage<UserMessageImpl>()
      ->SerializeIfNecessary();
}
