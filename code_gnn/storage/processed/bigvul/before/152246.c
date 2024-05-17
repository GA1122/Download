void RenderFrameImpl::DidBlockFramebust(const WebURL& url) {
  Send(new FrameHostMsg_DidBlockFramebust(GetRoutingID(), url));
}
