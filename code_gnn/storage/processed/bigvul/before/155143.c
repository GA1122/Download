void OmniboxViewViews::OnInlineAutocompleteTextCleared() {
  if (location_bar_view_)
    location_bar_view_->SetImeInlineAutocompletion(base::string16());
}
