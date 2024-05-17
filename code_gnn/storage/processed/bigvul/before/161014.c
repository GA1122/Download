void ChromeClientImpl::ShowVirtualKeyboardOnElementFocus(LocalFrame& frame) {
  WebLocalFrameImpl::FromFrame(frame.LocalFrameRoot())
      ->FrameWidget()
      ->Client()
      ->ShowVirtualKeyboardOnElementFocus();
}
