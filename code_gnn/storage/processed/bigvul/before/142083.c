HRESULT CGaiaCredentialBase::SetStringValue(DWORD field_id,
                                            const wchar_t* psz) {
  USES_CONVERSION;

  HRESULT hr = E_INVALIDARG;
  switch (field_id) {
    case FID_CURRENT_PASSWORD_FIELD:
      if (needs_windows_password_) {
        current_windows_password_ = W2COLE(psz);
        UpdateSubmitButtonInteractiveState();
      }
      hr = S_OK;
      break;
  }
  return hr;
}
