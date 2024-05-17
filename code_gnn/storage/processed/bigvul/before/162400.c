MojoResult Core::DestroyMessage(MojoMessageHandle message_handle) {
  if (!message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;

  RequestContext request_context;
  delete reinterpret_cast<ports::UserMessageEvent*>(message_handle);
  return MOJO_RESULT_OK;
}
