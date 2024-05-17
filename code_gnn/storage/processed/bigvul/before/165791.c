bool SVGElement::HasFocusEventListeners() const {
  return HasEventListeners(event_type_names::kFocusin) ||
         HasEventListeners(event_type_names::kFocusout) ||
         HasEventListeners(event_type_names::kFocus) ||
         HasEventListeners(event_type_names::kBlur);
}
