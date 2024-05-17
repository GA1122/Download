void RenderFrameHostImpl::AddMessageToConsole(
    blink::mojom::ConsoleMessageLevel level,
    const std::string& message) {
  Send(new FrameMsg_AddMessageToConsole(routing_id_, level, message));
}
