SVGElement::InstanceUpdateBlocker::~InstanceUpdateBlocker() {
  if (target_element_)
    target_element_->SetInstanceUpdatesBlocked(false);
}
