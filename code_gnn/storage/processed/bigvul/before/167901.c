Document* Document::open(LocalDOMWindow* entered_window,
                         const AtomicString& type,
                         const AtomicString& replace,
                         ExceptionState& exception_state) {
  open(entered_window->document(), exception_state);
  return this;
}
