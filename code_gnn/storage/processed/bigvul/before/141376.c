void Document::write(v8::Isolate* isolate,
                     const Vector<String>& text,
                     ExceptionState& exception_state) {
  if (IsTrustedTypesEnabledForDoc()) {
    DCHECK(origin_trials::TrustedDOMTypesEnabled(this));
    exception_state.ThrowTypeError(
        "This document can only write `TrustedHTML` objects.");
    return;
  }

  if (!AllowedToUseDynamicMarkUpInsertion("write", exception_state))
    return;

  StringBuilder builder;
  for (const String& string : text)
    builder.Append(string);
  write(builder.ToString(), EnteredDOMWindow(isolate)->document(),
        exception_state);
}
