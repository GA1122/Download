void HTMLInputElement::Trace(blink::Visitor* visitor) {
  visitor->Trace(input_type_);
  visitor->Trace(input_type_view_);
  visitor->Trace(list_attribute_target_observer_);
  visitor->Trace(image_loader_);
  TextControlElement::Trace(visitor);
}
