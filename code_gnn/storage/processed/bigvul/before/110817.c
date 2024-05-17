void AutocompleteEditModel::StopAutocomplete() {
  if (popup_->IsOpen() && !in_revert_) {
    InstantController* instant = controller_->GetInstant();
    if (instant && !instant->commit_on_mouse_up())
      instant->DestroyPreviewContents();
  }

  autocomplete_controller_->Stop(true);
}
