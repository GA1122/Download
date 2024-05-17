BSTR CGaiaCredentialBase::AllocErrorString(UINT id) {
  CComBSTR str(GetStringResource(id).c_str());
  return str.Detach();
}
