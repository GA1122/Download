void WebContext::setPopupBlockerEnabled(bool enabled) {
  if (IsInitialized()) {
    context_->SetIsPopupBlockerEnabled(enabled);
  } else {
    construct_props_->popup_blocker_enabled = enabled;
  }
}
