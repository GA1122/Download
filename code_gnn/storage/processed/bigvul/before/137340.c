bool MockInputMethod::HasComposition() {
  return composition_.text.length() || result_text_.length();
}
