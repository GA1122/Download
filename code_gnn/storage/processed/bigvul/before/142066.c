HRESULT CGaiaCredentialBase::GetStringValueImpl(DWORD field_id,
                                                wchar_t** value) {
  HRESULT hr = E_INVALIDARG;
  switch (field_id) {
    case FID_DESCRIPTION: {
      base::string16 description(
          GetStringResource(IDS_AUTH_FID_DESCRIPTION_BASE));
      hr = ::SHStrDupW(description.c_str(), value);
      break;
    }
    case FID_PROVIDER_LABEL: {
      base::string16 label(GetStringResource(IDS_AUTH_FID_PROVIDER_LABEL_BASE));
      hr = ::SHStrDupW(label.c_str(), value);
      break;
    }
    case FID_CURRENT_PASSWORD_FIELD: {
      hr = ::SHStrDupW(current_windows_password_.Length() > 0
                           ? current_windows_password_
                           : L"",
                       value);
      break;
    }
    default:
      break;
  }

  return hr;
}
