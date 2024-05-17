STDMETHODIMP FormatEtcEnumerator::Clone(IEnumFORMATETC** clone) {
  FormatEtcEnumerator* e = CloneFromOther(this);
  e->AddRef();
  *clone = e;
  return S_OK;
}
