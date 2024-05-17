void ChromeClientImpl::NotifyPopupOpeningObservers() const {
  const Vector<PopupOpeningObserver*> observers(popup_opening_observers_);
  for (const auto& observer : observers)
    observer->WillOpenPopup();
}
