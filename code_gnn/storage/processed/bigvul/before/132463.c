void UsbResetDeviceFunction::OnComplete(bool success) {
  if (success) {
    Respond(OneArgument(new base::FundamentalValue(true)));
  } else {
    scoped_refptr<UsbDeviceHandle> device_handle =
        GetDeviceHandle(parameters_->handle);
    if (device_handle.get()) {
      device_handle->Close();
    }
    ReleaseDeviceHandle(parameters_->handle);

    scoped_ptr<base::ListValue> error_args(new base::ListValue());
    error_args->AppendBoolean(false);
    Respond(ErrorWithArguments(error_args.Pass(), kErrorResetDevice));
  }
}
