void ChromeClientImpl::RegisterPopupOpeningObserver(
    PopupOpeningObserver* observer) {
  DCHECK(observer);
  popup_opening_observers_.push_back(observer);
}
