HRESULT DataObjectImpl::EnumFormatEtc(
    DWORD direction, IEnumFORMATETC** enumerator) {
  if (direction == DATADIR_GET) {
    FormatEtcEnumerator* e =
        new FormatEtcEnumerator(contents_.begin(), contents_.end());
    e->AddRef();
    *enumerator = e;
    return S_OK;
  }
  return E_NOTIMPL;
}
