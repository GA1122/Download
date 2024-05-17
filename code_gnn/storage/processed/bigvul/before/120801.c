void BluetoothDeviceChromeOS::Forget(const ErrorCallback& error_callback) {
  VLOG(1) << object_path_.value() << ": Removing device";
  DBusThreadManager::Get()->GetBluetoothAdapterClient()->
      RemoveDevice(
          adapter_->object_path_,
          object_path_,
          base::Bind(&base::DoNothing),
          base::Bind(&BluetoothDeviceChromeOS::OnForgetError,
                     weak_ptr_factory_.GetWeakPtr(),
                     error_callback));
}
