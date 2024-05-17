bool OmniboxViewViews::SelectedSuggestionHasTabMatch() const {
  return model()->popup_model() &&   
         model()->popup_model()->SelectedLineHasButton();
}
