GamepadDataFetcher* GamepadProvider::GetSourceGamepadDataFetcher(
    GamepadSource source) {
  for (GamepadFetcherVector::iterator it = data_fetchers_.begin();
       it != data_fetchers_.end();) {
    if ((*it)->source() == source) {
      return it->get();
    } else {
      ++it;
    }
  }
  return nullptr;
}
