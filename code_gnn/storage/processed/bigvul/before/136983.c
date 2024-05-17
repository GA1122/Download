void HTMLInputElement::SubtreeHasChanged() {
  input_type_view_->SubtreeHasChanged();
  CalculateAndAdjustDirectionality();
}
