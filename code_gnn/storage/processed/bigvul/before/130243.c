HRESULT DataObjectImpl::DAdvise(
    FORMATETC* format_etc, DWORD advf, IAdviseSink* sink, DWORD* connection) {
  return OLE_E_ADVISENOTSUPPORTED;
}
