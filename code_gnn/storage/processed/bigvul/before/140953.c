static bool AllDescendantsAreComplete(Frame* frame) {
  if (!frame)
    return true;
  for (Frame* child = frame->Tree().FirstChild(); child;
       child = child->Tree().TraverseNext(frame)) {
    if (child->IsLoading())
      return false;
  }
  return true;
}
