bool HTMLFormElement::isValidElement() {
  return !checkInvalidControlsAndCollectUnhandled(0,
                                                  CheckValidityDispatchNoEvent);
}
