void MockInputMethod::ClearComposition() {
  composition_ = ui::CompositionText();
  result_text_.clear();
}
