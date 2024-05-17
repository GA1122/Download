void BluetoothAdapterChromeOS::OnPropertyChangeCompleted(
    const base::Closure& callback,
    const ErrorCallback& error_callback,
    bool success) {
  if (success)
    callback.Run();
  else
    error_callback.Run();
}
