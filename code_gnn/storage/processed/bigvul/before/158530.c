void WebLocalFrameImpl::DispatchPrintEventRecursively(
    const AtomicString& event_type) {
  HeapVector<Member<Frame>> frames;
  for (Frame* frame = frame_; frame; frame = frame->Tree().TraverseNext(frame_))
    frames.push_back(frame);

  for (auto& frame : frames) {
    if (frame->IsRemoteFrame()) {
      continue;
    }
    if (!frame->Tree().IsDescendantOf(frame_))
      continue;
    ToLocalFrame(frame)->DomWindow()->DispatchEvent(*Event::Create(event_type));
  }
}
