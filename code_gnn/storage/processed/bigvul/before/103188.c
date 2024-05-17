void Browser::SetSuggestedText(const string16& text,
                               InstantCompleteBehavior behavior) {
  window()->GetLocationBar()->SetSuggestedText(text, behavior);
}
