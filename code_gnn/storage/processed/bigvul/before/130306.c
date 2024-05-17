HRESULT DataObjectImpl::StartOperation(IBindCtx* reserved) {
  async_operation_started_ = true;
  return S_OK;
}
