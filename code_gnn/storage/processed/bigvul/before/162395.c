ScopedMessagePipeHandle Core::CreatePartialMessagePipe(
    const ports::PortRef& port) {
  RequestContext request_context;
  return ScopedMessagePipeHandle(
      MessagePipeHandle(AddDispatcher(new MessagePipeDispatcher(
          GetNodeController(), port, kUnknownPipeIdForDebug, 1))));
}
