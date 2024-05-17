void UsbDeviceImpl::OnDisconnect() {
  DCHECK(thread_checker_.CalledOnValidThread());

  HandlesVector handles;
  swap(handles, handles_);

  for (const scoped_refptr<UsbDeviceHandleImpl>& handle : handles_) {
    handle->InternalClose();
  }
}
