void RenderFrameImpl::RequestFullscreenVideoElement() {
  WebElement video_element =
      frame_->GetDocument().GetElementsByHTMLTagName("video").FirstItem();

  if (!video_element.IsNull()) {
    blink::WebScopedUserGesture gesture(frame_);

    video_element.RequestFullscreen();
  }
}
