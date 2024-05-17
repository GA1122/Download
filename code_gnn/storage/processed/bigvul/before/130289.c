HRESULT DataObjectImpl::QueryInterface(const IID& iid, void** object) {
  if (!object)
    return E_POINTER;
  if (IsEqualIID(iid, IID_IDataObject) || IsEqualIID(iid, IID_IUnknown)) {
    *object = static_cast<IDataObject*>(this);
  } else if (in_async_mode_ &&
             IsEqualIID(iid, __uuidof(IDataObjectAsyncCapability))) {
    *object = static_cast<IDataObjectAsyncCapability*>(this);
  } else {
    *object = NULL;
    return E_NOINTERFACE;
  }
  AddRef();
  return S_OK;
}
