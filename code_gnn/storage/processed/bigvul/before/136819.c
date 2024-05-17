DOMWindow* LocalDOMWindow::open(ExecutionContext* executionContext,
                                LocalDOMWindow* current_window,
                                LocalDOMWindow* entered_window,
                                const String& url,
                                const AtomicString& target,
                                const String& features,
                                ExceptionState& exception_state) {
  if (!BindingSecurity::ShouldAllowAccessTo(entered_window, this,
                                            exception_state)) {
    UseCounter::Count(executionContext, WebFeature::kWindowOpenRealmMismatch);
    return nullptr;
  }
  DCHECK(!target.IsNull());
  return open(url, target, features, current_window, entered_window,
              exception_state);
}
