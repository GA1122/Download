void LocalFrame::SetIsInert(bool inert) {
  is_inert_ = inert;
  PropagateInertToChildFrames();
}
