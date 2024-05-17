  AutocompleteController* GetAutocompleteController() const {
    return GetLocationBar()->GetLocationEntry()->model()->popup_model()->
        autocomplete_controller();
  }
