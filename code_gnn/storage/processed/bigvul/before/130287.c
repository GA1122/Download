HRESULT DataObjectImpl::QueryGetData(FORMATETC* format_etc) {
  StoredData::const_iterator iter = contents_.begin();
  while (iter != contents_.end()) {
    if ((*iter)->format_etc.cfFormat == format_etc->cfFormat)
      return S_OK;
    ++iter;
  }
  return DV_E_FORMATETC;
}
