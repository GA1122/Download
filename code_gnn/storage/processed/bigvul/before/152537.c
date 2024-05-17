bool RenderFrameImpl::ShouldTrackUseCounter(const blink::WebURL& url) {
  return GetContentClient()->renderer()->ShouldTrackUseCounter(url);
}
