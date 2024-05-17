void Browser::SetSuggestedText(const string16& text) {
  window()->GetLocationBar()->SetSuggestedText(text);
}
