static std::string GetWindowTypeText(Browser::Type type) {
  if ((type & Browser::TYPE_APP) != 0 || type == Browser::TYPE_EXTENSION_APP)
    return keys::kWindowTypeValueApp;
  if ((type & Browser::TYPE_POPUP) != 0)
    return keys::kWindowTypeValuePopup;

  DCHECK(type == Browser::TYPE_NORMAL);
  return keys::kWindowTypeValueNormal;
}
