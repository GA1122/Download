void Document::writeln(v8::Isolate* isolate,
                       TrustedHTML* text,
                       ExceptionState& exception_state) {
  DCHECK(origin_trials::TrustedDOMTypesEnabled(this));
  writeln(text->toString(), EnteredDOMWindow(isolate)->document(),
          exception_state);
}
