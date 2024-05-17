bool ChromeClientImpl::RequestPointerLock(LocalFrame* frame) {
  LocalFrame& local_root = frame->LocalFrameRoot();
  return WebLocalFrameImpl::FromFrame(&local_root)
      ->FrameWidget()
      ->Client()
      ->RequestPointerLock();
}
