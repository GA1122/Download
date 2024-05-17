void Document::write(LocalDOMWindow* calling_window,
                     const Vector<String>& text,
                     ExceptionState& exception_state) {
  DCHECK(calling_window);
  StringBuilder builder;
  for (const String& string : text)
    builder.Append(string);
  write(builder.ToString(), calling_window->document(), exception_state);
}
