MojoResult Core::AttachMessageContext(MojoMessageHandle message_handle,
                                      uintptr_t context,
                                      MojoMessageContextSerializer serializer,
                                      MojoMessageContextDestructor destructor) {
  if (!message_handle || !context)
    return MOJO_RESULT_INVALID_ARGUMENT;
  auto* message = reinterpret_cast<ports::UserMessageEvent*>(message_handle)
                      ->GetMessage<UserMessageImpl>();
  return message->AttachContext(context, serializer, destructor);
}
