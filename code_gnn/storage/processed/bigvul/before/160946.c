void ChromeClientImpl::DetachCompositorAnimationTimeline(
    CompositorAnimationTimeline* compositor_timeline,
    LocalFrame* local_frame) {
  WebLocalFrameImpl* web_frame =
      WebLocalFrameImpl::FromFrame(local_frame)->LocalRoot();

  if (web_frame->FrameWidget()) {
    if (CompositorAnimationHost* animation_host =
            web_frame->FrameWidget()->AnimationHost())
      animation_host->RemoveTimeline(*compositor_timeline);
  }
}
