EventDispatchHandlingState* HTMLInputElement::PreDispatchEventHandler(
    Event* event) {
  if (event->type() == EventTypeNames::textInput &&
      input_type_view_->ShouldSubmitImplicitly(event)) {
    event->stopPropagation();
    return nullptr;
  }
  if (event->type() != EventTypeNames::click)
    return nullptr;
  if (!event->IsMouseEvent() ||
      ToMouseEvent(event)->button() !=
          static_cast<short>(WebPointerProperties::Button::kLeft))
    return nullptr;
  return input_type_view_->WillDispatchClick();
}
