HRESULT DataObjectImpl::InOperation(BOOL* in_async_op) {
  *in_async_op = async_operation_started_ ? TRUE : FALSE;
  return S_OK;
}
