void HTMLInputElement::SetListAttributeTargetObserver(
    ListAttributeTargetObserver* new_observer) {
  if (list_attribute_target_observer_)
    list_attribute_target_observer_->Unregister();
  list_attribute_target_observer_ = new_observer;
}
