void OmniboxViewViews::OnInlineAutocompleteTextCleared() {
  location_bar_view_->SetImeInlineAutocompletion(base::string16());
}
