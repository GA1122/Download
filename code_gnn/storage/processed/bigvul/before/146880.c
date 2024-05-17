void Document::close(ExceptionState& exception_state) {

  if (ImportLoader()) {
    exception_state.ThrowDOMException(
        kInvalidStateError, "Imported document doesn't support close().");
    return;
  }

  if (!IsHTMLDocument()) {
    exception_state.ThrowDOMException(kInvalidStateError,
                                      "Only HTML documents support close().");
    return;
  }

  if (throw_on_dynamic_markup_insertion_count_) {
    exception_state.ThrowDOMException(
        kInvalidStateError,
        "Custom Element constructor should not use close().");
    return;
  }

  close();
}
