HRESULT DataObjectImpl::GetAsyncMode(BOOL* is_op_async) {
  *is_op_async = in_async_mode_ ? TRUE : FALSE;
  return S_OK;
}
