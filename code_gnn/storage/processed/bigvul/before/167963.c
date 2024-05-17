void LocalFrame::PropagateInertToChildFrames() {
  for (Frame* child = Tree().FirstChild(); child;
       child = child->Tree().NextSibling()) {
    child->SetIsInert(is_inert_ ||
                      ToHTMLFrameOwnerElement(child->Owner())->IsInert());
  }
}
