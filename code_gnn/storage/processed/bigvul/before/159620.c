void Document::writeln(LocalDOMWindow* calling_window,
                       const Vector<String>& text,
                       ExceptionState& exception_state) {
  DCHECK(calling_window);

  if (GetSecurityContext().RequireTrustedTypes()) {
    DCHECK(RuntimeEnabledFeatures::TrustedDOMTypesEnabled());
    exception_state.ThrowTypeError(
        "This document can only write `TrustedHTML` objects.");
    return;
  }

  StringBuilder builder;
  for (const String& string : text)
    builder.Append(string);
  writeln(builder.ToString(), calling_window->document(), exception_state);
}
