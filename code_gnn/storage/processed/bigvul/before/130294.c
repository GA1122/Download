HRESULT DataObjectImpl::SetAsyncMode(BOOL do_op_async) {
  in_async_mode_ = (do_op_async == TRUE);
  return S_OK;
}
