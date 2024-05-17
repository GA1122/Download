String Document::OutgoingReferrer() const {

  const Document* referrer_document = this;

  if (GetSecurityOrigin()->IsOpaque())
    return String();

  if (LocalFrame* frame = frame_) {
    while (frame->GetDocument()->IsSrcdocDocument()) {
      frame = To<LocalFrame>(frame->Tree().Parent());
      DCHECK(frame);
    }
    referrer_document = frame->GetDocument();
  }

  return referrer_document->url_.StrippedForUseAsReferrer();
}
