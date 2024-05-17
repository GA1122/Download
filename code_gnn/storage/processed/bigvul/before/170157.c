void PasswordAutofillAgent::PasswordValueGatekeeper::OnUserGesture() {
  was_user_gesture_seen_ = true;

  for (blink::WebInputElement& element : elements_)
    ShowValue(&element);

  elements_.clear();
}
