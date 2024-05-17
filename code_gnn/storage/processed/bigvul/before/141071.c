LocalFrame* Document::ExecutingFrame() {
  LocalDOMWindow* window = ExecutingWindow();
  if (!window)
    return nullptr;
  return window->GetFrame();
}
