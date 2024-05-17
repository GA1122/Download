const UsbConfigDescriptor* UsbDeviceImpl::GetConfiguration() {
  DCHECK(thread_checker_.CalledOnValidThread());
  return configuration_.get();
}
