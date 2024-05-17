bool AutocompleteEditModel::query_in_progress() const {
  return !autocomplete_controller_->done();
}
