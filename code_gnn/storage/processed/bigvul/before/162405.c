ScopedMessagePipeHandle Core::ExtractMessagePipeFromInvitation(
    const std::string& name) {
  RequestContext request_context;
  ports::PortRef port0, port1;
  GetNodeController()->node()->CreatePortPair(&port0, &port1);
  MojoHandle handle = AddDispatcher(new MessagePipeDispatcher(
      GetNodeController(), port0, kUnknownPipeIdForDebug, 1));
  GetNodeController()->MergePortIntoParent(name, port1);
  return ScopedMessagePipeHandle(MessagePipeHandle(handle));
}
