SVGElement::InstanceUpdateBlocker::InstanceUpdateBlocker(
    SVGElement* target_element)
    : target_element_(target_element) {
  if (target_element_)
    target_element_->SetInstanceUpdatesBlocked(true);
}
