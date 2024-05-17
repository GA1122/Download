WebBluetoothServiceImpl::~WebBluetoothServiceImpl() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  ClearState();
}
