void OmniboxViewViews::DoInsertChar(base::char16 ch) {
  if ((model()->focus_state() == OMNIBOX_FOCUS_INVISIBLE) &&
      base::IsUnicodeWhitespace(ch)) {
    return;
  }

  if (insert_char_time_.is_null()) {
    DCHECK_EQ(latency_histogram_state_, NOT_ACTIVE);
    latency_histogram_state_ = CHAR_TYPED;
    insert_char_time_ = base::TimeTicks::Now();
  }
  Textfield::DoInsertChar(ch);
}
