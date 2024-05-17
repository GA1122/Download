bool RenderFrameImpl::ShouldBlockWebGL() {
  bool blocked = true;
  Send(new FrameHostMsg_Are3DAPIsBlocked(
      routing_id_, url::Origin(frame_->Top()->GetSecurityOrigin()).GetURL(),
      THREE_D_API_TYPE_WEBGL, &blocked));
  return blocked;
}
