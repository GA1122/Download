void Document::writeln(LocalDOMWindow* calling_window,
                       TrustedHTML* text,
                       ExceptionState& exception_state) {
  DCHECK(calling_window);
  DCHECK(RuntimeEnabledFeatures::TrustedDOMTypesEnabled());
  writeln(text->toString(), calling_window->document(), exception_state);
}
