void Document::write(LocalDOMWindow* calling_window,
                     TrustedHTML* text,
                     ExceptionState& exception_state) {
  DCHECK(calling_window);
  DCHECK(RuntimeEnabledFeatures::TrustedDOMTypesEnabled());
  write(text->toString(), calling_window->document(), exception_state);
}
