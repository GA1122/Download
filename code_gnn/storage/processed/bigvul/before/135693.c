void showTree(const blink::FrameSelection* sel) {
  if (sel)
    sel->ShowTreeForThis();
  else
    LOG(INFO) << "Cannot showTree for <null> FrameSelection.";
}
