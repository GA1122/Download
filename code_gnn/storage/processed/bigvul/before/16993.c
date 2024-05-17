bool WebContext::popupBlockerEnabled() const {
  if (IsInitialized()) {
    return context_->IsPopupBlockerEnabled();
  }

  return construct_props_->popup_blocker_enabled;
}
