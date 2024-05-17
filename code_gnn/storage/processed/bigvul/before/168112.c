  void CheckSuggestionsAvailableIfScreenReaderRunning() {
    EXPECT_EQ(has_active_screen_reader_,
              external_delegate_->has_suggestions_available_on_field_focus());
  }
