 void BluetoothDeviceChromeOS::OnDisconnect(const base::Closure& callback) {
   VLOG(1) << object_path_.value() << ": Disconnected";
   callback.Run();
}
