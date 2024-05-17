bool HTMLFormElement::checkValidity() {
  return !checkInvalidControlsAndCollectUnhandled(
      0, CheckValidityDispatchInvalidEvent);
}
