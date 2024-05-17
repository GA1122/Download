void ChromeClientImpl::UnregisterPopupOpeningObserver(
    PopupOpeningObserver* observer) {
  size_t index = popup_opening_observers_.Find(observer);
  DCHECK_NE(index, kNotFound);
  popup_opening_observers_.EraseAt(index);
}
