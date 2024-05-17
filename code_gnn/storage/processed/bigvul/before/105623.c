ui::KeyboardCode KeyMap::Get(const wchar_t& key) const {
  std::map<wchar_t, ui::KeyboardCode>::const_iterator it;
  it = keys_.find(key);
  if (it == keys_.end()) {
    it = shifted_keys_.find(key);
    if (it == shifted_keys_.end()) {
      return ui::VKEY_UNKNOWN;
    }
  }
  return it->second;
}
