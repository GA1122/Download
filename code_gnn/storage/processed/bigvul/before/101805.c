void Browser::SetSuggestedText(const string16& text,
                               InstantCompleteBehavior behavior) {
  if (window()->GetLocationBar())
    window()->GetLocationBar()->SetSuggestedText(text, behavior);
}
