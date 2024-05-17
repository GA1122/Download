HRESULT CGaiaCredentialBase::IsWindowsPasswordValidForStoredUser(
    BSTR password) const {
  if (username_.Length() == 0 || user_sid_.Length() == 0)
    return S_FALSE;

  if (::SysStringLen(password) == 0)
    return S_FALSE;
  OSUserManager* manager = OSUserManager::Get();
  return manager->IsWindowsPasswordValid(domain_, username_, password);
}
