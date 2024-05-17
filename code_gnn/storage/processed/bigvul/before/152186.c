void RenderFrameImpl::AddMessageToConsole(
    blink::mojom::ConsoleMessageLevel level,
    const std::string& message) {
  blink::WebConsoleMessage wcm(level, WebString::FromUTF8(message));
  frame_->AddMessageToConsole(wcm);
}
