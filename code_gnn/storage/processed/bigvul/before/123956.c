bool RenderViewImpl::allowWebGL(WebFrame* frame, bool default_value) {
  if (!default_value)
    return false;

  bool blocked = true;
  Send(new ViewHostMsg_Are3DAPIsBlocked(
      routing_id_,
      GURL(frame->top()->document().securityOrigin().toString()),
      THREE_D_API_TYPE_WEBGL,
      &blocked));
  return !blocked;
}
