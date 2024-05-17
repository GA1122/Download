void LocalFrame::DetachChildren() {
  DCHECK(loader_.StateMachine()->CreatingInitialEmptyDocument() ||
         GetDocument());

  if (Document* document = this->GetDocument())
    ChildFrameDisconnector(*document).Disconnect();
}
