bool IsTopLevelNavigation(WebFrame* frame) {
  return frame->Parent() == nullptr;
}
