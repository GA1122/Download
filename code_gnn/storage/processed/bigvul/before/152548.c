const std::string& UniqueNameForWebFrame(blink::WebFrame* frame) {
  return frame->IsWebLocalFrame()
             ? RenderFrameImpl::FromWebFrame(frame)->unique_name()
             : RenderFrameProxy::FromWebFrame(frame->ToWebRemoteFrame())
                   ->unique_name();
}
