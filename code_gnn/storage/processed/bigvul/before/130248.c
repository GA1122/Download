HRESULT DataObjectImpl::EndOperation(
    HRESULT result, IBindCtx* reserved, DWORD effects) {
  async_operation_started_ = false;
  return S_OK;
}
