bool HTMLFormElement::checkValidity() {
  return !CheckInvalidControlsAndCollectUnhandled(
      nullptr, kCheckValidityDispatchInvalidEvent);
}
