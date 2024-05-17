LocalFrame* Document::ExecutingFrame() {
  LocalDOMWindow* window = ExecutingWindow();
  if (!window)
    return 0;
  return window->GetFrame();
}
