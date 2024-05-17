bool HTMLFormElement::IsValidElement() {
  return !CheckInvalidControlsAndCollectUnhandled(
      nullptr, kCheckValidityDispatchNoEvent);
}
