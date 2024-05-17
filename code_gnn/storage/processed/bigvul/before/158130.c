void LocalFrameClientImpl::Trace(blink::Visitor* visitor) {
  visitor->Trace(web_frame_);
  LocalFrameClient::Trace(visitor);
}
