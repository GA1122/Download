void PasswordAutofillAgent::FirstUserGestureObserved() {
  gatekeeper_.OnUserGesture();
}
