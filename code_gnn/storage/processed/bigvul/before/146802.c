String Document::OutgoingReferrer() const {
  if (GetSecurityOrigin()->IsUnique()) {
    return String();
  }

  const Document* referrer_document = this;
  if (LocalFrame* frame = frame_) {
    while (frame->GetDocument()->IsSrcdocDocument()) {
      frame = ToLocalFrame(frame->Tree().Parent());
      DCHECK(frame);
    }
    referrer_document = frame->GetDocument();
  }
  return referrer_document->url_.StrippedForUseAsReferrer();
}
