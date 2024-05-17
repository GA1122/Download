void UsbDeviceImpl::OnOpenRequestComplete(const OpenCallback& callback,
                                          dbus::FileDescriptor fd) {
  blocking_task_runner_->PostTask(
      FROM_HERE, base::Bind(&UsbDeviceImpl::OpenOnBlockingThreadWithFd, this,
                             base::Passed(&fd), callback));
 }
