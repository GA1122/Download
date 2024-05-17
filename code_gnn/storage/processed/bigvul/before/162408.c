MojoResult Core::GetMessageContext(MojoMessageHandle message_handle,
                                   uintptr_t* context,
                                   MojoGetMessageContextFlags flags) {
  if (!message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;

  auto* message = reinterpret_cast<ports::UserMessageEvent*>(message_handle)
                      ->GetMessage<UserMessageImpl>();
  if (!message->HasContext())
    return MOJO_RESULT_NOT_FOUND;

  if (flags & MOJO_GET_MESSAGE_CONTEXT_FLAG_RELEASE)
    *context = message->ReleaseContext();
  else
    *context = message->context();
  return MOJO_RESULT_OK;
}
