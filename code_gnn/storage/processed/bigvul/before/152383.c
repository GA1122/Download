bool RenderFrameImpl::UniqueNameFrameAdapter::IsCandidateUnique(
    base::StringPiece name) const {
  DCHECK(!name.empty());

  for (blink::WebFrame* frame = GetWebFrame()->Top(); frame;
       frame = frame->TraverseNext()) {
    if (UniqueNameForWebFrame(frame) == name)
      return false;
  }

  return true;
}
