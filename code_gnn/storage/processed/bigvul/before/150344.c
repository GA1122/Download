void PasswordAutofillAgent::UserGestureObserved() {
  autofilled_elements_cache_.clear();

  gatekeeper_.OnUserGesture();
}
