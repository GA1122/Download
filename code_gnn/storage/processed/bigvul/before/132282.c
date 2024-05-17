blink::WebFrame* RenderFrameImpl::createChildFrame(
    blink::WebLocalFrame* parent,
    blink::WebTreeScopeType scope,
    const blink::WebString& name,
    blink::WebSandboxFlags sandbox_flags) {
  int child_routing_id = MSG_ROUTING_NONE;
  Send(new FrameHostMsg_CreateChildFrame(
      routing_id_, scope,
      base::UTF16ToUTF8(base::StringPiece16(name)), sandbox_flags,
      &child_routing_id));

  if (child_routing_id == MSG_ROUTING_NONE) {
    NOTREACHED() << "Failed to allocate routing id for child frame.";
    return nullptr;
  }

  RenderFrameImpl* child_render_frame = RenderFrameImpl::Create(
      render_view_.get(), child_routing_id);
  blink::WebLocalFrame* web_frame =
      WebLocalFrame::create(scope, child_render_frame);
  child_render_frame->SetWebFrame(web_frame);

  parent->appendChild(web_frame);
  child_render_frame->Initialize();

  return web_frame;
}
