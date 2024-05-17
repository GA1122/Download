MojoResult Core::CreateMessage(MojoMessageHandle* message_handle) {
  if (!message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;
  *message_handle = reinterpret_cast<MojoMessageHandle>(
      UserMessageImpl::CreateEventForNewMessage().release());
  return MOJO_RESULT_OK;
}
