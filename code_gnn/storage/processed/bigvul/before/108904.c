static bool IsTopLevelNavigation(WebFrame* frame) {
  return frame->parent() == NULL;
}
