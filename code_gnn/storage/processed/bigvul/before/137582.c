blink::WebFrame* PrepareFrameAndViewForPrint::createChildFrame(
    blink::WebLocalFrame* parent,
    blink::WebTreeScopeType scope,
    const blink::WebString& name,
    blink::WebSandboxFlags sandboxFlags) {
  blink::WebFrame* frame = blink::WebLocalFrame::create(scope, this);
  parent->appendChild(frame);
  return frame;
}
