HRESULT CGaiaCredentialBase::GetStringValue(DWORD field_id, wchar_t** value) {
  return GetStringValueImpl(field_id, value);
}
