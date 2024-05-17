void Document::write(v8::Isolate* isolate,
                     TrustedHTML* text,
                     ExceptionState& exception_state) {
  DCHECK(origin_trials::TrustedDOMTypesEnabled(this));
  write(text->toString(), EnteredDOMWindow(isolate)->document(),
        exception_state);
}
