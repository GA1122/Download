Frame* LocalFrame::FindFrameForNavigation(const AtomicString& name,
                                          LocalFrame& active_frame,
                                          const KURL& destination_url) {
  Frame* frame = Tree().Find(name);
  if (!frame || !active_frame.CanNavigate(*frame, destination_url))
    return nullptr;
  return frame;
}
