void AutofillDialogViews::SetValidityForInput(
    T* input,
    const base::string16& message) {
  bool invalid = !message.empty();
  input->SetInvalid(invalid);

  if (invalid) {
    validity_map_[input] = message;
  } else {
    validity_map_.erase(input);

    if (error_bubble_ &&
        error_bubble_->anchor()->GetAncestorWithClassName(
            input->GetClassName()) == input) {
      validity_map_.erase(input);
      HideErrorBubble();
    }
  }
}
