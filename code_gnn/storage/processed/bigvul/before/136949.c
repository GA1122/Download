void HTMLInputElement::RemovedFrom(ContainerNode* insertion_point) {
  input_type_view_->ClosePopupView();
  if (insertion_point->isConnected() && !Form())
    RemoveFromRadioButtonGroup();
  TextControlElement::RemovedFrom(insertion_point);
  DCHECK(!isConnected());
  ResetListAttributeTargetObserver();
}
