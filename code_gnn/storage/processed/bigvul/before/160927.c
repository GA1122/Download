void ChromeClientImpl::AttachCompositorAnimationTimeline(
    CompositorAnimationTimeline* compositor_timeline,
    LocalFrame* local_frame) {
  WebLocalFrameImpl* web_frame =
      WebLocalFrameImpl::FromFrame(local_frame)->LocalRoot();
  if (CompositorAnimationHost* animation_host =
          web_frame->FrameWidget()->AnimationHost())
    animation_host->AddTimeline(*compositor_timeline);
}
